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

    static std::map<AVSampleFormat, audio::sample_format> sample_formats = {
            {AV_SAMPLE_FMT_FLT, audio::sample_format::FLT}
    };

    static video::image_format getVideoFormat(AVPixelFormat av_pixel_format) {
        if (!video_formats.contains(av_pixel_format)) {
            return video::image_format::unknown;
        }
        return video_formats.at(av_pixel_format);
    }

    static audio::sample_format getAudioFormat(AVSampleFormat av_sample_format) {
        if (!sample_formats.contains(av_sample_format)) {
            return audio::sample_format::unknown;
        }
        return sample_formats.at(av_sample_format);
    }

    Fill::Fill(const stream_sptr& stream) {
        this->_stream = stream;
    }

    misc::vector_sptr<frame_sptr> Fill::filter(const misc::vector_sptr<frame_sptr> &in) {
        auto stream = this->_stream.lock();
        for (auto& frame : *in) {
            auto raw_frame = frame->raw();
            if (stream->_first) {
                frame->first = true;
                stream->_first = false;
            }
            frame->time_base = stream->timeBase();
            frame->pts = raw_frame->pts;

            if (stream->av_codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
                frame->img_fmt = getVideoFormat(static_cast<AVPixelFormat>(raw_frame->format));
            } else if (stream->av_codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO) {
                frame->sample_fmt = getAudioFormat(static_cast<AVSampleFormat>(raw_frame->format));
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
