//
// Created by CrabExcavator on 2020/11/20.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_TICKER_H_
#define PLAYER_SRC_MISC_TICKER_H_

#include <cstdint>
#include <chrono>
#include <thread>

#include "common/Error.h"

namespace misc {

class Ticker {
 public:
  common::Error Init(uint64_t hz) {
    auto ret = common::Error::SUCCESS;

    this->hz_ = hz;
    this->tick_ = 0;
    this->tick_duration_ =
        std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::duration<double>(1.0/this->hz_));
    this->last_tick_point_ = std::chrono::steady_clock::now();
    return ret;
  }

  void Wait() {
    std::this_thread::sleep_until(this->last_tick_point_ + this->tick_duration_);
    this->last_tick_point_ = std::chrono::steady_clock::now();
  }

  void AdjustHZ(uint64_t hz) {
    this->hz_ = hz;
  }

  uint64_t HZ() {
    return this->hz_;
  }

  uint64_t CurrentTick() {
    return this->tick_;
  }

 private:
  uint64_t hz_;
  uint64_t tick_;
  std::chrono::steady_clock::duration tick_duration_;
  std::chrono::steady_clock::time_point last_tick_point_;
};

}

#endif //PLAYER_SRC_MISC_TICKER_H_
