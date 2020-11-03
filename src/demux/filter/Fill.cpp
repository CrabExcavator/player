//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <map>

#include "Fill.h"
#include "demux/Stream.h"

namespace demux::filter {

    static std::map<AVPixelFormat, video::image_format> video_formats = {
            {AV_PIX_FMT_YUV420P, video::image_format::yuv420p}
    };

    static std::map<AVSampleFormat, audio::sample_format> sample_formats = {
            {AV_SAMPLE_FMT_FLTP, audio::sample_format::fltp}
    };

    static std::map<audio::sample_format, int> sample_format_size = {
            {audio::sample_format::fltp, 4}
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

    static int getSampleSize(audio::sample_format fmt) {
        assert(fmt != audio::sample_format::unknown);
        return sample_format_size.at(fmt);
    }

    Fill::Fill(const stream_sptr& stream) {
        this->_stream = stream;
    }

    common::error Fill::filter(const misc::vector_sptr<frame_sptr> &in, misc::vector_sptr<frame_sptr>& out) {
        auto stream = this->_stream.lock();
        if (stream == nullptr) {
            return common::error::streamUnknownError;
        }
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
                frame->sample_size = getSampleSize(frame->sample_fmt);
                frame->num_of_channel = raw_frame->channels;
                frame->num_of_sample = raw_frame->nb_samples;
                frame->sample_rate = raw_frame->sample_rate;
            }
        }
        out = in;
        return common::error::success;
    }

    common::error Fill::flush(const misc::vector_sptr<frame_sptr> &in, misc::vector_sptr<frame_sptr>& out) {
        return this->filter(in, out);
    }

    common::error Fill::close() {
        return common::error::success;
    }

}
