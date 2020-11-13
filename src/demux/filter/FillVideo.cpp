//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <map>

#include "FillVideo.h"
#include "demux/stream/IStream.h"

namespace demux::filter {

static std::map<AVPixelFormat, video::ImageFormat> video_formats = {
    {AV_PIX_FMT_YUV420P, video::ImageFormat::yuv420p}
};

static std::map<audio::SampleFormat, int> sample_format_size = {
    {audio::SampleFormat::FLTP, 4}
};

static video::ImageFormat getVideoFormat(AVPixelFormat av_pixel_format) {
  if (!video_formats.contains(av_pixel_format)) {
    return video::ImageFormat::unknown;
  }
  return video_formats.at(av_pixel_format);
}

static int getSampleSize(audio::SampleFormat fmt) {
  assert(fmt != audio::SampleFormat::UNKNOWN);
  return sample_format_size.at(fmt);
}

common::Error FillVideo::filter(const misc::vector_sptr<frame_sptr> &in, misc::vector_sptr<frame_sptr> &out) {
  for (auto &frame : *in) {
    auto raw_frame = frame->raw();
    frame->pts = raw_frame->pts;
    frame->img_fmt = getVideoFormat(static_cast<AVPixelFormat>(raw_frame->format));
  }
  out = in;
  return common::Error::SUCCESS;
}

common::Error FillVideo::flush(const misc::vector_sptr<frame_sptr> &in, misc::vector_sptr<frame_sptr> &out) {
  return this->filter(in, out);
}

common::Error FillVideo::close() {
  return common::Error::SUCCESS;
}

}
