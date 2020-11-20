//
// Created by CrabExcavator on 2020/11/12.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <map>
#include <glog/logging.h>

#include "FFFrame.h"

namespace demux::frame {

static std::map<AVSampleFormat, output::audio::SampleFormat> AVSampleFormatMap = {
    {AV_SAMPLE_FMT_FLTP, output::audio::SampleFormat::FLTP}
};

static std::map<AVPixelFormat, video::ImageFormat> AVImageFormatMap = {
    {AV_PIX_FMT_YUV420P, video::ImageFormat::yuv420p}
};

FFFrame::FFFrame() :
av_frame_(nullptr),
first_(false),
last_(false),
sample_format_attribute_(nullptr),
image_format_attribute_(nullptr) {}

common::Error FFFrame::Init(AVFrame *av_frame, bool first, bool last) {
  auto ret = common::Error::SUCCESS;

  if (first && last) {
    LOG(WARNING) << "first && last all set true";
    ret = common::Error::INVALID_ARGS;
  } else if (!last && (av_frame == nullptr)) {
    LOG(WARNING) << "not last frame but av_frame is null";
    ret = common::Error::INVALID_ARGS;
  } else if (last && (av_frame != nullptr)) {
    LOG(WARNING) << "last frame is not null";
    ret = common::Error::INVALID_ARGS;
  }

  if (common::Error::SUCCESS != ret) {
    // do nothing
  } else {
    this->av_frame_ = av_frame;
    this->first_ = first;
    this->last_ = last;
  }

  return ret;
}

FFFrame::~FFFrame() {
  av_frame_free(&this->av_frame_);
  this->av_frame_ = nullptr;
}

bool FFFrame::IsFirst() {
  return this->first_;
}

bool FFFrame::IsLast() {
  return this->last_;
}

common::Error FFFrame::GetData(misc::vector_sptr<common::Slice> &data) {
  auto ret = common::Error::SUCCESS;

  assert(data == nullptr);
  if (data != nullptr) {
    ret = common::Error::INVALID_ARGS;
  }
  data = std::make_shared<std::vector<common::Slice>>();

  if (common::Error::SUCCESS != ret) {
    // do nothing
  } else {
    int i = 0;
    for(; this->av_frame_->data[i] != nullptr ; i++) {
      common::Slice slice(this->av_frame_->data[i], this->av_frame_->linesize[i]);
      data->emplace_back(slice);
    }
  }

  return ret;
}

int64_t FFFrame::GetPts() {
  return this->av_frame_->pts;
}

video::ImageFormat FFFrame::GetImageFormat() {
  if (this->image_format_attribute_ == nullptr) {
    auto av_image_format = static_cast<AVPixelFormat>(this->av_frame_->format);
    if (AVImageFormatMap.contains(av_image_format)) {
      this->image_format_attribute_ =
          video::ImageFormatAttributeMap[AVImageFormatMap[av_image_format]];
    }
  }
  return this->image_format_attribute_ == nullptr
  ? video::ImageFormat::unknown : this->image_format_attribute_->image_format;
}

int FFFrame::GetWidth() {
  return this->av_frame_->width;
}

int FFFrame::GetHeight() {
  return this->av_frame_->height;
}

output::audio::SampleFormat FFFrame::GetSampleFormat() {
  if (this->sample_format_attribute_ == nullptr) {
    auto av_sample_format = static_cast<AVSampleFormat>(this->av_frame_->format);
    if (AVSampleFormatMap.contains(av_sample_format)) {
      this->sample_format_attribute_ =
          output::audio::SampleFormatAttributeMap[AVSampleFormatMap[av_sample_format]];
    }
  }
  return this->sample_format_attribute_ == nullptr
  ? output::audio::SampleFormat::UNKNOWN : this->sample_format_attribute_->sample_format;
}

int FFFrame::GetSampleSize() {
  if (this->sample_format_attribute_ == nullptr) {
    auto av_sample_format = static_cast<AVSampleFormat>(this->av_frame_->format);
    if (AVSampleFormatMap.contains(av_sample_format)) {
      this->sample_format_attribute_ =
          output::audio::SampleFormatAttributeMap[AVSampleFormatMap[av_sample_format]];
    }
  }
  return this->sample_format_attribute_ == nullptr
  ? 0 : this->sample_format_attribute_->sample_size;
}

int FFFrame::GetNumOfChannel() {
  return this->av_frame_->channels;
}

int FFFrame::GetNumOfSample() {
  return this->av_frame_->nb_samples;
}

int FFFrame::GetSampleRate() {
  return this->av_frame_->sample_rate;
}

}