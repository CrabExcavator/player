//
// Created by CrabExcavator on 2020/11/12.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_DEMUX_FRAME_FFFRAME_H_
#define PLAYER_SRC_DEMUX_FRAME_FFFRAME_H_

#include "IFrame.h"
#include "misc/avheader.h"

namespace demux::frame {

class FFFrame : public IFrame {
 public:
  FFFrame();

  ~FFFrame() override;

  /**
   * @brief init && take control of AVFrame
   * @param [in] av_frame
   * @param [in] first
   * @param [in] last
   * @return error code
   */
  common::Error Init(AVFrame *av_frame, bool first, bool last);

  bool IsFirst() override;

  bool IsLast() override;

  common::Error GetData(misc::vector_sptr<common::Slice>& data) override;

  int64_t GetPts() override;

  output::video::ImageFormat GetImageFormat() override;

  int GetWidth() override;

  int GetHeight() override;

  output::audio::SampleFormat GetSampleFormat() override;

  int GetSampleSize() override;

  int GetNumOfChannel() override;

  int GetNumOfSample() override;

  int GetSampleRate() override;

 private:
  bool first_;

  bool last_;

  AVFrame *av_frame_;

  output::video::ImageFormatAttribute* image_format_attribute_;

  output::audio::SampleFormatAttribute* sample_format_attribute_;
};

}

#endif //PLAYER_SRC_DEMUX_FRAME_FFFRAME_H_
