//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <map>

#include "FillVideo.h"
#include "demux/stream/Stream.h"

namespace demux::filter {

    static std::map<AVPixelFormat, video::image_format> video_formats = {
            {AV_PIX_FMT_YUV420P, video::image_format::yuv420p}
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

    static int getSampleSize(audio::sample_format fmt) {
        assert(fmt != audio::sample_format::unknown);
        return sample_format_size.at(fmt);
    }

    common::error FillVideo::filter(const misc::vector_sptr<frame_sptr> &in, misc::vector_sptr<frame_sptr>& out) {
        for (auto& frame : *in) {
            auto raw_frame = frame->raw();
            frame->pts = raw_frame->pts;
            frame->img_fmt = getVideoFormat(static_cast<AVPixelFormat>(raw_frame->format));
        }
        out = in;
        return common::error::success;
    }

    common::error FillVideo::flush(const misc::vector_sptr<frame_sptr> &in, misc::vector_sptr<frame_sptr>& out) {
        return this->filter(in, out);
    }

    common::error FillVideo::close() {
        return common::error::success;
    }

}
