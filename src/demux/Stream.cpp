//
// Created by weihan on 2020/10/11.
//

#include <memory>
#include <thread>

#include "Stream.h"
#include "filter/Blit.h"
#include "exception/InitException.h"

namespace demux {

    static void avcodec_free_context_wrapper(AVCodecContext* ctx) {
        avcodec_free_context(&ctx);
    }

    void Stream::init(const std::shared_ptr<AVFormatContext>& av_fmt_ctx_, int index, std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> queue_) {
        this->_index = index;
        this->queue = std::move(std::move(queue_));
        this->_av_fmt_ctx = av_fmt_ctx_;
        auto codec_param = av_fmt_ctx_->streams[index]->codecpar;
        auto codec = avcodec_find_decoder(codec_param->codec_id);
        av_codec_ctx_uptr av_codec_ctx_(avcodec_alloc_context3(codec), avcodec_free_context_wrapper);
        bool success = true;
        success = (avcodec_parameters_to_context(av_codec_ctx_.get(), codec_param) >= 0);
        if (success) {
            success = (avcodec_open2(av_codec_ctx_.get(), codec, nullptr) >= 0);
        }
        if (success) {
            this->av_codec_ctx.swap(av_codec_ctx_);
        }
        if (!success) {
            throw exception::StreamInitException();
        }
        this->_frame_filter_chain = std::make_shared<misc::Chain<frame_sptr>>();
        this->_frame_filter_chain
        ->addLast(std::make_shared<filter::Fill>(shared_from_this()))
        ->addLast(std::make_shared<filter::Blit>());
        this->_first = true;
    }

    void Stream::feed(const av_packet_sptr &packet) {
        int ret = (avcodec_send_packet(this->av_codec_ctx.get(), packet.get()) >= 0);
        while (ret >= 0) {
            this->_frame = std::make_shared<Frame>();
            ret = avcodec_receive_frame(this->av_codec_ctx.get(), this->_frame->raw());
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                break;
            }
            if (ret < 0) {
                break;
            }
            misc::vector_sptr<frame_sptr> in = std::make_shared<std::vector<frame_sptr>>();
            in->emplace_back(this->_frame);
            this->_frame = this->_frame_filter_chain->filter(in)->at(0);
            this->queue->blockingWrite(this->_frame);
        }
    }

    std::chrono::nanoseconds Stream::timeBase() const {
        // todo check if demuxer is null
        auto seconds = std::chrono::duration<double>(av_q2d(this->_av_fmt_ctx->streams[this->_index]->time_base));
        return std::chrono::duration_cast<std::chrono::nanoseconds>(seconds);
    }

}
