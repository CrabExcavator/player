//
// Created by CrabExcavator on 2021/1/1.
// Copyright (c) 2021 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_LATCH_H_
#define PLAYER_SRC_MISC_LATCH_H_

#include <cstdint>
#include <mutex>

namespace misc {

class Latch {
 public:
  Latch():
  latch_(0)
  {}

  void Add(int64_t count) {
    latch_ += count;
    if (count < 0) {
      cond_.notify_one();
    }
  }

  void Done() {
    latch_ -= 1;
    cond_.notify_one();
  }

  void Wait() {
    std::unique_lock lock(mutex_);
    cond_.wait(lock, [&](){
      return latch_ <= 0;
    });
  }

 private:
  std::atomic<int64_t> latch_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

}

#endif //PLAYER_SRC_MISC_LATCH_H_
