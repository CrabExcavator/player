//
// Created by weihan on 2020/10/18.
//

#include <map>

#include "Fill.h"
#include "demux/Stream.h"

namespace demux::filter {

    static std::map<AVPixelFormat, video::image_format> video_formats = {
            {AV_PIX_FMT_YUV420P, video::image_format::yuv420p}
    };

    static video::image_format getVideoFormat(AVPixelFormat av_pixel_format) {
        if (!video_formats.contains(av_pixel_format)) {
            return video::image_format::unknown;
        }
        return video_formats.at(av_pixel_format);
    }

    Fill::Fill(const stream_sptr& stream) {
        this->_stream = stream;
    }

    misc::vector_sptr<frame_sptr> Fill::filter(const misc::vector_sptr<frame_sptr> &in) {
        auto stream = this->_stream.lock();
        for (auto& frame : *in) {
            if (stream->_first) {
                frame->first = true;
                stream->_first = false;
            }
            frame->time_base = stream->timeBase();
            frame->pts = frame->raw()->pts;

            // video only todo add audio support
            if (stream->av_codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
                frame->imgfmt = getVideoFormat(static_cast<AVPixelFormat>(frame->raw()->format));
            }
        }
        return in;
    }

    misc::vector_sptr<frame_sptr> Fill::flush(const misc::vector_sptr<frame_sptr> &in) {
        return this->filter(in);
    }

    void Fill::close() {

    }

}
