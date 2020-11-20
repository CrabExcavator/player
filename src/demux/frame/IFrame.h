//
// Created by CrabExcavator on 2020/11/12.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_DEMUX_FRAME_IFRAME_H_
#define PLAYER_SRC_DEMUX_FRAME_IFRAME_H_

#include <chrono>

#include "output/audio/SampleFormat.h"
#include "output/video/ImageFormat.h"
#include "common/Error.h"
#include "misc/Slice.h"
#include "misc/typeptr.h"

namespace demux::frame {

class IFrame {
 public:
  virtual ~IFrame() = default;

  //////////////////////////////          common          //////////////////////////////

  virtual bool IsFirst() {
    return false;
  }

  virtual bool IsLast() {
    return false;
  }

  /**
   * @brief get data
   * @param [out] data
   * @return error code
   */
  virtual common::Error GetData(misc::vector_sptr<common::Slice>& data) = 0;

  virtual int64_t GetPts() = 0;



  //////////////////////////////          video          //////////////////////////////

  virtual output::video::ImageFormat GetImageFormat() {
    return output::video::ImageFormat::unknown;
  }

  virtual int GetWidth() {
    return 0;
  }

  virtual int GetHeight() {
    return 0;
  }



  //////////////////////////////          audio          //////////////////////////////

  virtual output::audio::SampleFormat GetSampleFormat() {
    return output::audio::SampleFormat::UNKNOWN;
  }

  virtual int GetSampleSize() {
    return 0;
  }

  virtual int GetNumOfChannel() {
    return 0;
  }

  virtual int GetNumOfSample() {
    return 0;
  }

  virtual int GetSampleRate() {
    return 0;
  }

};

}

#endif //PLAYER_SRC_DEMUX_FRAME_IFRAME_H_
