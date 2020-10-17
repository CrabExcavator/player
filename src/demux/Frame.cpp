//
// Created by weihan on 2020/10/11.
//

#include <map>

#include "Frame.h"
#include "demux/Stream.h"
#include "video/image_format.h"

namespace demux {

    Frame::Frame() {
        this->_frame = av_frame_alloc();
    }

    Frame::~Frame() {
        av_frame_free(&this->_frame);
        free(this->pixels);
    }

    static std::map<AVPixelFormat, video::image_format> video_formats = {
            {AV_PIX_FMT_YUV420P, video::image_format::yuv420p}
    };

    static video::image_format getVideoFormat(AVPixelFormat av_pixel_format) {
        if (!video_formats.contains(av_pixel_format)) {
            return video::image_format::unknown;
        }
        return video_formats.at(av_pixel_format);
    }

    void Frame::fill(const demux::stream_sptr& stream) {
        if (stream->av_codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
            this->imgfmt = getVideoFormat(static_cast<AVPixelFormat>(this->_frame->format));
        }
    }

}
