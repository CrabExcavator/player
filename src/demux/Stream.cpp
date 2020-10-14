//
// Created by weihan on 2020/10/11.
//

#include <memory>

#include "Stream.h"
#include "exception/InitException.h"

namespace demux {

    static void avcodec_free_context_wrapper(AVCodecContext* ctx) {
        avcodec_free_context(&ctx);
    }

    Stream::Stream(const std::shared_ptr<Demuxer>& demuxer, int index, std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> queue) {
        this->_index = index;
        this->_demuxer = demuxer;
        this->_queue = std::move(queue);
        auto codec_param = demuxer->_av_format_ctx->streams[index]->codecpar;
        auto codec = avcodec_find_decoder(codec_param->codec_id);
        av_codec_ctx_uptr av_codec_ctx(avcodec_alloc_context3(codec), avcodec_free_context_wrapper);
        bool success = true;
        success = (avcodec_parameters_to_context(av_codec_ctx.get(), codec_param) >= 0);
        if (success) {
            success = (avcodec_open2(av_codec_ctx.get(), codec, nullptr) >= 0);
        }
        if (success) {
            this->_av_codec_ctx.swap(av_codec_ctx);
        }
        if (!success) {
            throw exception::StreamInitException();
        }
    }

    void Stream::feed(const av_packet_sptr &packet) {
        int ret = 0;
        ret = (avcodec_send_packet(this->_av_codec_ctx.get(), packet.get()) >= 0);
        if (ret >= 0) {
            this->_frame = std::make_shared<Frame>();
            ret = avcodec_receive_frame(this->_av_codec_ctx.get(), this->_frame->get());
        }
        if (ret >= 0) {
            this->_queue->write(this->_frame);
            //LOG(INFO) << "push stream pts: " << this->_frame->get()->pts;
        }
    }

}
