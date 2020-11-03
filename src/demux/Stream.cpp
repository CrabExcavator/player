//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <memory>
#include <thread>

#include "Stream.h"
#include "filter/Blit.h"
#include "exception/InitException.h"
#include "demux/DemuxContext.h"
#include "misc/Chain.h"

namespace demux {

    static void avcodec_free_context_wrapper(AVCodecContext* ctx) {
        avcodec_free_context(&ctx);
    }

    common::error Stream::init(const std::shared_ptr<AVFormatContext>& av_fmt_ctx_, int index, const demux_ctx_sptr& demux_ctx) {
        bool success = true;
        this->_index = index;
        this->_av_fmt_ctx = av_fmt_ctx_;
        this->_frame_filter_chain = std::make_shared<misc::Chain<frame_sptr>>();
        auto codec_param = av_fmt_ctx_->streams[index]->codecpar;
        if (codec_param->codec_type == AVMEDIA_TYPE_VIDEO) {
            this->queue = demux_ctx->vo_queue;
            this->_frame_filter_chain
            ->addLast(std::make_shared<filter::Fill>(shared_from_this()))
            ->addLast(std::make_shared<filter::Blit>())
            ;
        } else if (codec_param->codec_type == AVMEDIA_TYPE_AUDIO) {
            this->queue = demux_ctx->ao_queue;
            this->_frame_filter_chain
            ->addLast(std::make_shared<filter::Fill>(shared_from_this()))
            ->addLast(std::make_shared<filter::Blit>())
            ;
        } else {
            success = false;
        }
        auto codec = avcodec_find_decoder(codec_param->codec_id);
        av_codec_ctx_uptr av_codec_ctx_(avcodec_alloc_context3(codec), avcodec_free_context_wrapper);
        if (success) {
            success = (avcodec_parameters_to_context(av_codec_ctx_.get(), codec_param) >= 0);
        }
        if (success) {
            success = (avcodec_open2(av_codec_ctx_.get(), codec, nullptr) >= 0);
        }
        if (success) {
            this->av_codec_ctx.swap(av_codec_ctx_);
        }
        if (!success) {
            throw exception::StreamInitException();
        }
        this->_first = true;
        this->av_codec_ctx->channel_layout = av_get_default_channel_layout(this->av_codec_ctx->channels);
        return common::error::success;
    }

    common::error Stream::feed(const av_packet_sptr &packet) {
        auto err = common::error::success;
        int avErr = (avcodec_send_packet(this->av_codec_ctx.get(), packet.get()) >= 0);
        while (avErr >= 0) {
            this->_frame = std::make_shared<Frame>();
            avErr = avcodec_receive_frame(this->av_codec_ctx.get(), this->_frame->raw());
            if (avErr == AVERROR(EAGAIN) || avErr == AVERROR_EOF) {
                break;
            } else if (avErr < 0) {
                return common::error::streamFeedPacketFail;
            }
            misc::vector_sptr<frame_sptr> in = std::make_shared<std::vector<frame_sptr>>();
            misc::vector_sptr<frame_sptr> out{};
            in->emplace_back(this->_frame);
            if ((err = this->_frame_filter_chain->filter(in, out)) != common::error::success) {
                return err;
            }
            if (out == nullptr || out->size() != 1) {
                LOG(FATAL) << "unexpected filtered list of frame";
                return common::error::streamFeedPacketFail;
            }
            this->_frame = out->at(0);
            this->queue->blockingWrite(this->_frame);
        }
        return err;
    }

    common::error Stream::close() {
        this->_frame = std::make_shared<Frame>();
        this->_frame->last = true;
        this->queue->blockingWrite(this->_frame);
        return common::error::success;
    }

    std::chrono::nanoseconds Stream::timeBase() const {
        // todo check if demuxer is null
        auto seconds = std::chrono::duration<double>(av_q2d(this->_av_fmt_ctx->streams[this->_index]->time_base));
        return std::chrono::duration_cast<std::chrono::nanoseconds>(seconds);
    }

}
