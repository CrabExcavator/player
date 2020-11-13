//
// Created by CrabExcavator on 2020/11/5.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <map>

#include "FillAudio.h"

namespace demux::filter {

static std::map<AVSampleFormat, audio::SampleFormat> sample_formats = {
    {AV_SAMPLE_FMT_FLTP, audio::SampleFormat::FLTP}
};

static std::map<audio::SampleFormat, int> sample_format_size = {
    {audio::SampleFormat::FLTP, 4}
};

static audio::SampleFormat getAudioFormat(AVSampleFormat av_sample_format) {
  if (!sample_formats.contains(av_sample_format)) {
    return audio::SampleFormat::UNKNOWN;
  }
  return sample_formats.at(av_sample_format);
}

static int getSampleSize(audio::SampleFormat fmt) {
  assert(fmt != audio::SampleFormat::UNKNOWN);
  return sample_format_size.at(fmt);
}

common::Error FillAudio::filter(const misc::vector_sptr<demux::frame_sptr> &in,
                                misc::vector_sptr<demux::frame_sptr> &out) {
  for (auto &frame : *in) {
    auto raw_frame = frame->raw();
    frame->pts = raw_frame->pts;
    frame->sample_fmt = getAudioFormat(static_cast<AVSampleFormat>(raw_frame->format));
    frame->sample_size = getSampleSize(frame->sample_fmt);
    frame->num_of_channel = raw_frame->channels;
    frame->num_of_sample = raw_frame->nb_samples;
    frame->sample_rate = raw_frame->sample_rate;
  }
  out = in;
  return common::Error::SUCCESS;
}

common::Error FillAudio::flush(const misc::vector_sptr<demux::frame_sptr> &in,
                               misc::vector_sptr<demux::frame_sptr> &out) {
  return this->filter(in, out);
}

common::Error FillAudio::close() {
  return common::Error::SUCCESS;
}

}