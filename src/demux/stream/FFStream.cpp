//
// Created by CrabExcavator on 2020/11/9.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "FFStream.h"
#include "common/Config.h"
#include "demux/Frame.h"
#include "misc/Chain.h"
#include "demux/filter/FillVideo.h"
#include "demux/filter/Blit.h"
#include "demux/filter/FillAudio.h"

namespace demux::stream {

    /// @todo put in misc/avheader.h
    static void avcodec_free_context_wrapper(AVCodecContext* ctx) {
        avcodec_free_context(&ctx);
    }

    common::error FFStream::init(const AVStream *stream) {
        auto codecParm = stream->codecpar;
        if (codecParm->codec_type == AVMEDIA_TYPE_VIDEO) {
            this->_op = core::output_port::video;
            this->_frame_filter_chain = std::make_shared<misc::Chain<frame_sptr>>();
            this->_frame_filter_chain
                ->addLast(std::make_shared<filter::FillVideo>())
                ->addLast(std::make_shared<filter::Blit>())
                ;

        } else if (codecParm->codec_type == AVMEDIA_TYPE_AUDIO) {
            this->_op = core::output_port::audio;
            this->_frame_filter_chain = std::make_shared<misc::Chain<frame_sptr>>();
            this->_frame_filter_chain
                ->addLast(std::make_shared<filter::FillAudio>())
                ;
        }
        this->_codec = avcodec_find_decoder(codecParm->codec_id);
        av_codec_ctx_uptr codec_ctx(avcodec_alloc_context3(this->_codec), avcodec_free_context_wrapper);
        this->_codec_ctx.swap(codec_ctx);
        if (avcodec_parameters_to_context(this->_codec_ctx.get(), codecParm) < 0) {
            LOG(ERROR) << "avcodec_parameters_to_context fail";
            return common::error::unknownError;
        } else if (avcodec_open2(this->_codec_ctx.get(), this->_codec, nullptr) < 0) {
            LOG(ERROR) << "avcodec_open2 fail";
            return common::error::unknownError;
        }
        this->_codec_ctx->channel_layout = av_get_default_channel_layout(this->_codec_ctx->channels);
        this->_time_base = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::duration<double>(av_q2d(stream->time_base)));
        this->_queue = std::make_shared<folly::MPMCQueue<frame_sptr>>(GET_CONFIG(default_queue_size));
        return common::error::success;
    }

    common::error FFStream::read(demux::frame_sptr &frame) {
        return this->_queue->read(frame) ? common::error::success : common::error::unRead;
    }

    common::error FFStream::close() {
        auto frame = std::make_shared<Frame>();
        frame->last = true;
        this->_queue->blockingWrite(frame);
        return common::error::success;
    }

    common::error FFStream::feed(const av_packet_sptr &packet) {
        auto err = common::error::success;
        if (avcodec_send_packet(this->_codec_ctx.get(), packet.get()) < 0) {
            return common::error::unknownError;
        }
        int av_err = 0;
        do {
            auto frame = std::make_shared<Frame>();
            av_err = avcodec_receive_frame(this->_codec_ctx.get(), frame->raw());
            if (av_err == AVERROR(EAGAIN) || av_err == AVERROR_EOF) {
                break;
            } else if (av_err < 0){
                return common::error::unknownError;
            }

            if (this->_frame_filter_chain != nullptr) {
                auto in = std::make_shared<std::vector<frame_sptr>>();
                misc::vector_sptr<frame_sptr> out = nullptr;
                in->emplace_back(frame);
                if ((err = this->_frame_filter_chain->filter(in, out)) != common::error::success) {
                    return err;
                }
                assert(out != nullptr);
                assert(out->size() == 1);
            }
            if (this->_first) {
                this->_first = false;
                frame->first = true;
            }
            this->_queue->blockingWrite(frame);
        } while(av_err >= 0);
        return err;
    }

    std::chrono::nanoseconds FFStream::getTimeBase() const {
        return this->_time_base;
    }

    core::output_port FFStream::getOutputPort() const {
        return this->_op;
    }

}