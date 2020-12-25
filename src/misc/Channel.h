//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_CHANNEL_H_
#define PLAYER_SRC_MISC_CHANNEL_H_

#include <mutex>
#include <deque>

namespace misc {

template<size_t Size, typename T>
class Channel {
 public:
  Channel() = default;
  Channel(const Channel &rhs) = delete;
  Channel(Channel &&rhs) noexcept:
  deque_(std::move(rhs.deque_)) {}
  Channel &operator = (const Channel &rhs) = delete;
  Channel &operator = (Channel &&rhs) noexcept {
    deque_ = std::move(rhs.deque_);
  }

  void BlockingPut(T ele) {
    deque_.push_front(std::move(ele));
  }

  void BlockingGet(T &ele) {
    ele = *deque_.rbegin();
    deque_.pop_back();
  }

 private:
  std::deque<T> deque_;
};

}

#endif //PLAYER_SRC_MISC_CHANNEL_H_
