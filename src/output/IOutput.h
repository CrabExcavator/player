//
// Created by CrabExcavator on 2020/11/14.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_OUTPUT_IOUTPUT_H_
#define PLAYER_SRC_OUTPUT_IOUTPUT_H_

#include "misc/Runnable.h"
#include "misc/ThreadPool.h"

namespace output {

class IOutput : public misc::Runnable {
 public:
  ~IOutput() override = default;

  common::Error Run() override = 0;
};

}

#endif //PLAYER_SRC_OUTPUT_IOUTPUT_H_
