//
// Created by CrabExcavator on 2020/11/27.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_DEMUX_FRAME_RESAMPLEDFRAME_H_
#define PLAYER_SRC_DEMUX_FRAME_RESAMPLEDFRAME_H_

#include "IFrame.h"
#include "tool/resample/IResample.h"

namespace demux::frame {

class ResampledFrame : public IFrame {
 public:
  ResampledFrame();
  ~ResampledFrame() override = default;
  common::Error Init(const frame_sptr& frame, const tool::resample::resample_output_sptr& resample_output);
  bool IsFirst() override;
  bool IsLast() override;
  common::Error GetData(misc::vector_sptr<misc::Slice> &data) override;
  int64_t GetPts() override;
  output::video::ImageFormat GetImageFormat() override;
  int GetWidth() override;
  int GetHeight() override;
  output::audio::SampleFormat GetSampleFormat() override;
  int GetSampleSize() override;
  int GetNumOfChannel() override;
  int GetNumOfSample() override;
  int GetSampleRate() override;
  int GetAudioLineSize() override;
  output::audio::ChannelLayout GetChannelLayout() override;
  common::Error DoResample(const tool::resample::resample_sptr &resample,
                           tool::resample::resample_output_sptr &resample_output_sptr) override;

 private:
  frame_sptr frame_;
  tool::resample::resample_output_sptr resample_output_;
  misc::vector_sptr<misc::Slice> resample_data_;
  tool::resample::Desc resample_desc_;
  output::audio::SampleFormatAttribute *sample_format_attribute_;
};

}

#endif //PLAYER_SRC_DEMUX_FRAME_RESAMPLEDFRAME_H_
