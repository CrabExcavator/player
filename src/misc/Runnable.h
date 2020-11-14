//
// Created by CrabExcavator on 2020/11/14.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_RUNNABLE_H_
#define PLAYER_SRC_MISC_RUNNABLE_H_

#include "common/Error.h"

namespace misc {

class Runnable {
 public:
  virtual ~Runnable() = default;

  virtual common::Error Run() = 0;
};

}

#endif //PLAYER_SRC_MISC_RUNNABLE_H_
