//
// Created by CrabExcavator on 2020/11/20.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_LOOPER_H_
#define PLAYER_SRC_MISC_LOOPER_H_

#include <cstdint>

#include "Ticker.h"

namespace misc {

template <uint64_t hz>
class Looper {
 public:
  Looper() {
    this->ticker_.Init(hz);
  }

  virtual ~Looper() = default;

 protected:
  virtual bool LoopImpl() = 0;

  bool Loop() {
    this->ticker_.Wait();
    return this->LoopImpl();
  }

  void AdjustHZ(uint64_t _hz) {
    this->ticker_.AdjustHZ(_hz);
  }

 private:
  Ticker ticker_;
};

}

#endif //PLAYER_SRC_MISC_LOOPER_H_
