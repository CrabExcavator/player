//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_CHANNEL_H_
#define PLAYER_SRC_MISC_CHANNEL_H_

#include <mutex>
#include <condition_variable>
#include <deque>

namespace misc {

template<size_t Size, typename T>
class Channel {
 public:
  Channel() = default;

  Channel(const Channel &rhs) = delete;

  Channel(Channel &&rhs) noexcept:
  mutex_(std::move(rhs.mutex_)),
  cond_(std::move(rhs.cond_)),
  deque_(std::move(rhs.deque_)){}

  Channel &operator = (const Channel &rhs) = delete;

  Channel &operator = (Channel &&rhs) noexcept {
    mutex_ = std::move(rhs.mutex_);
    cond_ = std::move(rhs.cond_);
    deque_ = std::move(rhs.deque_);
  }

  void BlockingPut(T ele) {
    std::unique_lock lock(mutex_);
    cond_.wait(lock, [&](){
      return deque_.size() < Size;
    });
    deque_.emplace_front(std::move(ele));
    lock.unlock();
    cond_.notify_one();
  }

  void BlockingGet(T &ele) {
    std::unique_lock lock(mutex_);
    cond_.wait(lock, [&]() {
      return deque_.size() > 0;
    });
    ele = std::move(*deque_.rbegin());
    deque_.pop_back();
    lock.unlock();
    cond_.notify_one();
  }

  bool Get(T &ele) {
    auto ret = false;
    std::unique_lock lock(mutex_);

    if (deque_.empty()) {
      ret = false;
    } else {
      ele = *deque_.rbegin();
      deque_.pop_back();
      ret = true;
    }
    lock.unlock();
    cond_.notify_one();
    return ret;
  }

 private:
  std::mutex mutex_;
  std::condition_variable cond_;
  std::deque<T> deque_;
};

template<typename T>
class Channel<0, T> {
 public:
  Channel() = delete;
};

}

#endif //PLAYER_SRC_MISC_CHANNEL_H_
