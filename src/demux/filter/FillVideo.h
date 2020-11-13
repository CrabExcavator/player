//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_FILLVIDEO_H
#define PLAYER_FILLVIDEO_H

#include <memory>

#include "FrameFilterBase.h"
#include "misc/typeptr.h"

namespace demux::filter {

/**
 * @brief setNumOfStream var in frame
 */
class FillVideo : public FrameFilterBase {
 public:
  FillVideo() = default;

  common::Error filter(const misc::vector_sptr<frame_sptr> &in, misc::vector_sptr<frame_sptr> &out) override;

  common::Error flush(const misc::vector_sptr<frame_sptr> &in, misc::vector_sptr<frame_sptr> &out) override;

  common::Error close() override;
};

}

#endif //PLAYER_FILLVIDEO_H
