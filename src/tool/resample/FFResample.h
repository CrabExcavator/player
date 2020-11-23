//
// Created by CrabExcavator on 2020/11/23.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_UTIL_RESAMPLE_FFRESAMPLE_H_
#define PLAYER_SRC_UTIL_RESAMPLE_FFRESAMPLE_H_

#include <glog/logging.h>

#include "IResample.h"
#include "common/Error.h"
#include "misc/avheader.h"

namespace tool::resample {

class FFResample : public IResample {
 public:
  common::Error Init();

 private:
  swr_ctx_uptr swr_ctx_;
};

}

#endif //PLAYER_SRC_UTIL_RESAMPLE_FFRESAMPLE_H_
