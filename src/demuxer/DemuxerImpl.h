//
// Created by weihan on 2020/10/9.
//

#ifndef PLAYER_DEMUXERIMPL_H
#define PLAYER_DEMUXERIMPL_H

#include <memory>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include "Demuxer.h"
#include "core/PlayerContext.h"

namespace demuxer {

    class DemuxerImpl {
    public:
        friend class Demuxer;
        DemuxerImpl() = default;
        ~DemuxerImpl();
        void init();

    private:
        std::unique_ptr<AVFormatContext> _av_format_ctx;
        std::unique_ptr<AVCodecContext> _av_codec_ctx;
        std::unique_ptr<AVCodec> _av_codec;
        std::unique_ptr<AVFrame> _av_frame;
        std::unique_ptr<AVFrame> _av_frame_yuv;
        std::unique_ptr<AVPacket> _av_packet;
    };

}

#endif //PLAYER_DEMUXERIMPL_H
