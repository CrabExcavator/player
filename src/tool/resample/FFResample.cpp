//
// Created by CrabExcavator on 2020/11/23.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "FFResample.h"

namespace tool::resample {

FFResampleOutput::~FFResampleOutput() {
  if (nullptr != data_) {
    av_freep(&data_[0]);
  }
  av_freep(&data_);
}

common::Error FFResampleOutput::Init(uint8_t **data, Desc desc) {
  auto ret = common::Error::SUCCESS;

  data_ = data;
  desc_ = desc;
  return ret;
}

common::Error FFResampleOutput::GetData(misc::vector_sptr<misc::Slice> &out) {
  auto ret = common::Error::SUCCESS;
  assert(nullptr == out);

  out = std::make_shared<std::vector<misc::Slice>>();
  for (int i = 0 ; i < desc_.number_of_channel ; i++) {
    misc::Slice slice(data_[i], desc_.linesize);
    out->emplace_back(slice);
  }
  return ret;
}

common::Error FFResampleOutput::GetDesc(Desc &desc) {
  auto ret = common::Error::SUCCESS;

  desc = desc_;
  return ret;
}

common::Error FFResample::Init(Desc src, Desc dst) {
  auto ret = common::Error::SUCCESS;
  /// @todo verify pass-in arguments

  swr_ctx_uptr swr_ctx(swr_alloc(), swr_free_wrapper);
  if (nullptr == swr_ctx) {
    LOG(WARNING) << "swr alloc fail";
    ret = common::Error::UNKNOWN_ERROR;
  } else if (0 != av_opt_set_int(swr_ctx.get(), "in_channel_layout",
                                 ChannelLayoutTranslate(src.layout), 0)) {
    LOG(WARNING) << "src channel layout set fail";
    ret = common::Error::UNKNOWN_ERROR;
  } else if (0 != av_opt_set_int(swr_ctx.get(), "in_sample_rate",
                                 src.sample_rate, 0)) {
    LOG(WARNING) << "src sample rate set fail";
    ret = common::Error::UNKNOWN_ERROR;
  } else if (0 != av_opt_set_sample_fmt(swr_ctx.get(), "in_sample_fmt",
                                        SampleFormatTranslate(src.sample_format), 0)) {
    LOG(WARNING) << "src sample fmt set fail";
    ret = common::Error::UNKNOWN_ERROR;
  } else if (0 != av_opt_set_int(swr_ctx.get(), "out_channel_layout",
                                 ChannelLayoutTranslate(dst.layout), 0)) {
    LOG(WARNING) << "dst channel layout set fail";
    ret = common::Error::UNKNOWN_ERROR;
  } else if (0 != av_opt_set_int(swr_ctx.get(), "out_sample_rate",
                                 dst.sample_rate, 0)) {
    LOG(WARNING) << "dst sample rate set fail";
    ret = common::Error::UNKNOWN_ERROR;
  } else if (0 != av_opt_set_sample_fmt(swr_ctx.get(), "out_sample_fmt",
                                        SampleFormatTranslate(dst.sample_format), 0)) {
    LOG(WARNING) << "dst sample fmt set fail";
    ret = common::Error::UNKNOWN_ERROR;
  } else if (0 > swr_init(swr_ctx.get())) {
    LOG(WARNING) << "swr init fail";
    ret = common::Error::UNKNOWN_ERROR;
  } else {
      src_ = src;
      dst_ = dst;
      swr_ctx_.swap(swr_ctx);
  }
  return ret;
}

common::Error FFResample::operator()(const uint8_t **src_data,
                                     int number_of_sample,
                                     int line_size,
                                     resample_output_sptr &resample_output) {
  auto ret = common::Error::SUCCESS;
  assert(resample_output == nullptr);
  auto ff_resample_output = std::make_shared<FFResampleOutput>();
  uint8_t **dst_data = nullptr;

  /// src_sample_rate * src_number_of_sample = dst_sample_rate * dst_number_of_sample
  dst_.number_of_sample =
      av_rescale_rnd(number_of_sample, dst_.sample_rate, src_.sample_rate, AV_ROUND_UP);
  if (0 > av_samples_alloc_array_and_samples(&dst_data,
                                             &dst_.linesize,
                                             dst_.number_of_channel,
                                             dst_.number_of_sample,
                                             SampleFormatTranslate(dst_.sample_format),
                                             0)) {
    ret = common::Error::OUT_OF_MEMORY;
  } else if (0 > swr_convert(swr_ctx_.get(), dst_data, dst_.number_of_sample,
                             src_data, number_of_sample)) {
    LOG(WARNING) << "swr convert fail";
    ret = common::Error::UNKNOWN_ERROR;
  } else {
    ff_resample_output->Init(dst_data, dst_);
    resample_output = ff_resample_output;
  }

  return ret;
}

uint64_t FFResample::ChannelLayoutTranslate(output::audio::ChannelLayout channel_layout) {
  uint64_t ret = 0x0000000000000000;

  if (output::audio::ChannelLayout::STEREO == channel_layout) {
    ret = AV_CH_LAYOUT_STEREO;
  } else if (output::audio::ChannelLayout::_5POINT1_BACK == channel_layout) {
    ret = AV_CH_LAYOUT_5POINT1_BACK;
  }
  return ret;
}

AVSampleFormat FFResample::SampleFormatTranslate(output::audio::SampleFormat sample_format) {
  auto ret = AV_SAMPLE_FMT_NONE;

  if (output::audio::SampleFormat::FLTP == sample_format) {
    ret = AV_SAMPLE_FMT_FLTP;
  }
  return ret;
}

void FFResample::fill_samples(double *dst, int nb_samples, int nb_channels, int sample_rate, double *t) {
  int i, j;
  double tincr = 1.0 / sample_rate, *dstp = dst;
  const double c = 2 * M_PI * 440.0;
  /* generate sin tone with 440Hz frequency and duplicated channels */
  for (i = 0; i < nb_samples; i++) {
    *dstp = sin(c * *t);
    for (j = 1; j < nb_channels; j++)
      dstp[j] = dstp[0];
    dstp += nb_channels;
    *t += tincr;
  }
}

}