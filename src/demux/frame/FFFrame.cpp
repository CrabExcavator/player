//
// Created by CrabExcavator on 2020/11/12.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <map>
#include <glog/logging.h>

#include "FFFrame.h"
#include "tool/resample/IResample.h"

namespace demux::frame {

static std::map<AVSampleFormat, output::audio::SampleFormat> AVSampleFormatMap = {
    {AV_SAMPLE_FMT_FLTP, output::audio::SampleFormat::FLTP}
};

static std::map<AVPixelFormat, output::video::ImageFormat> AVImageFormatMap = {
    {AV_PIX_FMT_YUV420P, output::video::ImageFormat::yuv420p}
};

FFFrame::FFFrame():
    av_frame_(nullptr),
    first_(false),
    last_(false),
    sample_format_attribute_(nullptr),
    image_format_attribute_(nullptr),
    resample_output_(nullptr),
    resample_data_(nullptr),
    resample_desc_() {}

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
    av_frame_ = av_frame;
    first_ = first;
    last_ = last;
  }

  return ret;
}

FFFrame::~FFFrame() {
  av_frame_free(&av_frame_);
  av_frame_ = nullptr;
}

bool FFFrame::IsFirst() {
  return first_;
}

bool FFFrame::IsLast() {
  return last_;
}

common::Error FFFrame::GetData(misc::vector_sptr<misc::Slice> &data) {
  auto ret = common::Error::SUCCESS;

  assert(data == nullptr);
  if (data != nullptr) {
    ret = common::Error::INVALID_ARGS;
  }
  data = std::make_shared<std::vector<misc::Slice>>();

  if (common::Error::SUCCESS != ret) {
    // do nothing
  } else {
    if (nullptr != image_format_attribute_) {
      if (output::video::ImageFormat::yuv420p == image_format_attribute_->image_format) {
        int size_y = av_frame_->height * av_frame_->linesize[0];
        int size_uv = ((av_frame_->height + 1) / 2) * ((av_frame_->linesize[0] + 1) / 2);
        misc::Slice slice_y(av_frame_->data[0], size_y);
        misc::Slice slice_u(av_frame_->data[1], size_uv);
        misc::Slice slice_v(av_frame_->data[2], size_uv);
        data->emplace_back(slice_y);
        data->emplace_back(slice_u);
        data->emplace_back(slice_v);
      }
    } else if (nullptr != sample_format_attribute_) {
      if (nullptr != resample_output_) {
        data = resample_data_;
      } else {
        for (int i = 0; av_frame_->data[i] != nullptr && i < AV_NUM_DATA_POINTERS; i++) {
          misc::Slice slice(av_frame_->data[i], GetAudioLineSize());
          data->emplace_back(slice);
        }
      }
    }
  }

  return ret;
}

int64_t FFFrame::GetPts() {
  return av_frame_->pts;
}

output::video::ImageFormat FFFrame::GetImageFormat() {
  if (image_format_attribute_ == nullptr) {
    auto av_image_format = static_cast<AVPixelFormat>(av_frame_->format);
    if (AVImageFormatMap.contains(av_image_format)) {
      image_format_attribute_ =
          output::video::ImageFormatAttributeMap[AVImageFormatMap[av_image_format]];
    }
  }
  return image_format_attribute_ == nullptr
  ? output::video::ImageFormat::unknown : image_format_attribute_->image_format;
}

int FFFrame::GetWidth() {
  return av_frame_->width;
}

int FFFrame::GetHeight() {
  return av_frame_->height;
}

output::audio::SampleFormat FFFrame::GetSampleFormat() {
  if (nullptr != resample_output_) {
    return resample_desc_.sample_format;
  }
  if (sample_format_attribute_ == nullptr) {
    auto av_sample_format = static_cast<AVSampleFormat>(av_frame_->format);
    if (AVSampleFormatMap.contains(av_sample_format)) {
      sample_format_attribute_ =
          output::audio::SampleFormatAttributeMap[AVSampleFormatMap[av_sample_format]];
    }
  }
  return sample_format_attribute_ == nullptr
  ? output::audio::SampleFormat::UNKNOWN : sample_format_attribute_->sample_format;
}

int FFFrame::GetSampleSize() {
  if (sample_format_attribute_ == nullptr) {
    auto av_sample_format = static_cast<AVSampleFormat>(av_frame_->format);
    if (AVSampleFormatMap.contains(av_sample_format)) {
      sample_format_attribute_ =
          output::audio::SampleFormatAttributeMap[AVSampleFormatMap[av_sample_format]];
    }
  }
  return sample_format_attribute_ == nullptr
  ? 0 : sample_format_attribute_->sample_size;
}

int FFFrame::GetNumOfChannel() {
  return nullptr == resample_output_ ? av_frame_->channels : resample_desc_.number_of_channel;
}

int FFFrame::GetNumOfSample() {
  return nullptr == resample_output_ ? av_frame_->nb_samples : resample_desc_.number_of_sample;
}

int FFFrame::GetSampleRate() {
  return nullptr == resample_output_ ? av_frame_->sample_rate : resample_desc_.sample_rate;
}

int FFFrame::GetAudioLineSize() {
  return nullptr == resample_output_ ? av_frame_->linesize[0] : resample_desc_.linesize;
}

output::audio::ChannelLayout FFFrame::GetChannelLayout() {
  auto ret = output::audio::ChannelLayout::UNKNOWN;

  if (nullptr != resample_output_) {
    ret = resample_desc_.layout;
  } else {
    if (AV_CH_LAYOUT_STEREO == av_frame_->channel_layout) {
      ret = output::audio::ChannelLayout::STEREO;
    } else if (AV_CH_LAYOUT_SURROUND == av_frame_->channel_layout) {
      ret = output::audio::ChannelLayout::SURROUND;
    } else if (AV_CH_LAYOUT_5POINT1 == av_frame_->channel_layout) {
      ret = output::audio::ChannelLayout::_5POINT1;
    } else if (AV_CH_LAYOUT_5POINT1_BACK == av_frame_->channel_layout) {
      ret = output::audio::ChannelLayout::_5POINT1_BACK;
    }
  }
  return ret;
}

common::Error FFFrame::DoResample(const tool::resample::resample_sptr &resample,
                                  tool::resample::resample_output_sptr &resample_output) {
  auto ret = common::Error::SUCCESS;
  assert(resample_output == nullptr);

  //resample_output_ = nullptr;
  if (common::Error::SUCCESS !=
  (ret = (*resample)((const uint8_t **)av_frame_->data,
      av_frame_->nb_samples, GetAudioLineSize(), resample_output))) {
    LOG(WARNING) << "resample fail";
  } else {
    //resample_output_->GetData(resample_data_);
    //resample_output_->GetDesc(resample_desc_);
  }
  return ret;
}

}