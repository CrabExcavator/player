//
// Created by CrabExcavator on 2020/11/5.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <map>

#include "FillAudio.h"

namespace demux::filter {

    static std::map<AVSampleFormat, audio::sample_format> sample_formats = {
            {AV_SAMPLE_FMT_FLTP, audio::sample_format::fltp}
    };

    static std::map<audio::sample_format, int> sample_format_size = {
            {audio::sample_format::fltp, 4}
    };

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

    common::error FillAudio::filter(const misc::vector_sptr<demux::frame_sptr> &in,
                                                   misc::vector_sptr<demux::frame_sptr> &out) {
        for (auto& frame : *in) {
            auto raw_frame = frame->raw();
            frame->pts = raw_frame->pts;
            frame->sample_fmt = getAudioFormat(static_cast<AVSampleFormat>(raw_frame->format));
            frame->sample_size = getSampleSize(frame->sample_fmt);
            frame->num_of_channel = raw_frame->channels;
            frame->num_of_sample = raw_frame->nb_samples;
            frame->sample_rate = raw_frame->sample_rate;
        }
        out = in;
        return common::error::success;
    }

    common::error FillAudio::flush(const misc::vector_sptr<demux::frame_sptr> &in,
                                                  misc::vector_sptr<demux::frame_sptr> &out) {
        return this->filter(in, out);
    }

    common::error FillAudio::close() {
        return common::error::success;
    }

}