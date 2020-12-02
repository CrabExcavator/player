//
// Created by CrabExcavator on 2020/11/27.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <glog/logging.h>

#include "ResampledFrame.h"

namespace demux::frame {

ResampledFrame::ResampledFrame():
frame_(nullptr),
resample_output_(nullptr),
resample_data_(nullptr),
resample_desc_(),
sample_format_attribute_(nullptr){}

common::Error ResampledFrame::Init(const frame_sptr& frame, const tool::resample::resample_output_sptr& resample_output) {
  auto ret = common::Error::SUCCESS;

  if (nullptr == frame || nullptr == resample_output) {
    ret = common::Error::INVALID_ARGS;
  } else if (common::Error::SUCCESS != (ret = resample_output->GetData(resample_data_))) {
    LOG(WARNING) << "get resample data fail";
  } else if (common::Error::SUCCESS != (ret = resample_output->GetDesc(resample_desc_))) {
    LOG(WARNING) << "get resample desc fail";
  } else {
    frame_ = frame;
    resample_output_ = resample_output;
  }
  return ret;
}

bool ResampledFrame::IsFirst() {
  return frame_->IsFirst();
}

bool ResampledFrame::IsLast() {
  return frame_->IsLast();
}

common::Error ResampledFrame::GetData(misc::vector_sptr<misc::Slice> &data) {
  auto ret = common::Error::SUCCESS;

  if (nullptr != resample_output_) {
    data = resample_data_;
  } else if (common::Error::SUCCESS != (ret = frame_->GetData(data))) {
    LOG(WARNING) << "get data from resample frame fail";
  }
  return ret;
}

int64_t ResampledFrame::GetPts() {
  return frame_->GetPts();
}

output::video::ImageFormat ResampledFrame::GetImageFormat() {
  return frame_->GetImageFormat();
}

int ResampledFrame::GetWidth() {
  return frame_->GetWidth();
}

int ResampledFrame::GetHeight() {
  return frame_->GetHeight();
}

output::audio::SampleFormat ResampledFrame::GetSampleFormat() {
  auto ret = output::audio::SampleFormat::UNKNOWN;

  if (nullptr != resample_output_) {
    ret = resample_desc_.sample_format;
  } else {
    ret = frame_->GetSampleFormat();
  }
  return ret;
}

int ResampledFrame::GetSampleSize() {
  if (nullptr == sample_format_attribute_) {
    auto sample_format = GetSampleFormat();
    sample_format_attribute_ = output::audio::SampleFormatAttributeMap[sample_format];
  }
  return nullptr != sample_format_attribute_ ? sample_format_attribute_->sample_size : 0;
}

int ResampledFrame::GetNumOfChannel() {
  auto ret = 0;

  if (nullptr != resample_output_) {
    ret = resample_desc_.number_of_channel;
  } else {
    ret = frame_->GetNumOfChannel();
  }
  return ret;
}

int ResampledFrame::GetNumOfSample() {
  auto ret = 0;

  if (nullptr != resample_output_) {
    ret = resample_desc_.number_of_sample;
  } else {
    ret = frame_->GetNumOfSample();
  }
  return ret;
}

int ResampledFrame::GetSampleRate() {
  auto ret = 0;

  if (nullptr != resample_output_) {
    ret = resample_desc_.sample_rate;
  } else {
    ret = frame_->GetSampleRate();
  }
  return ret;
}

int ResampledFrame::GetAudioLineSize() {
  auto ret = 0;

  if (nullptr != resample_output_) {
    ret = resample_desc_.linesize;
  } else {
    ret = frame_->GetAudioLineSize();
  }
  return ret;
}

output::audio::ChannelLayout ResampledFrame::GetChannelLayout() {
  auto ret = output::audio::ChannelLayout::UNKNOWN;

  if (nullptr != resample_output_) {
    ret = resample_desc_.layout;
  } else {
    ret = frame_->GetChannelLayout();
  }
  return ret;
}

common::Error ResampledFrame::DoResample(const tool::resample::resample_sptr &resample,
                                         tool::resample::resample_output_sptr &resample_output_sptr) {
  return common::Error::UNKNOWN_ERROR;
}

}
