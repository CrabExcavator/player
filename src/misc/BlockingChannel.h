//
// Created by CrabExcavator on 2020/12/26.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_BLOCKINGCHANNEL_H_
#define PLAYER_SRC_MISC_BLOCKINGCHANNEL_H_

#include <deque>
#include <memory>
#include <mutex>
#include <condition_variable>

namespace misc {

template<typename T>
class BlockingChannel {
 public:
  BlockingChannel() = default;

  BlockingChannel(const BlockingChannel &rhs) = delete;

  BlockingChannel(BlockingChannel &&rhs) noexcept:
  mutex_(std::move(rhs.mutex_)),
  cond_(std::move(rhs.cond_)),
  deque_(std::move(rhs.deque_)) {}

  BlockingChannel &operator = (const BlockingChannel &rhs) = delete;

  BlockingChannel &operator = (BlockingChannel &&rhs) noexcept {
    mutex_ = std::move(rhs.mutex_);
    cond_ = std::move(rhs.cond_);
    deque_ = std::move(rhs.deque_);
  }

  BlockingChannel &operator << (T ele) {
    BlockingPut(ele);
    return *this;
  }

  BlockingChannel &operator >> (T &ele) {
    BlockingGet(ele);
    return *this;
  }

  void BlockingPut(T ele) {
    auto ele_wrapper = std::make_shared<EleWrapper>();
    ele_wrapper->ele = std::move(ele);
    ele_wrapper->consumed = false;
    std::unique_lock lock(mutex_);
    deque_.emplace_front(ele_wrapper);
    lock.unlock();
    cond_.notify_one();

    /// wait until ele consumed
    std::unique_lock ele_lock(ele_wrapper->mutex);
    ele_wrapper->cond.wait(ele_lock, [&](){
      return ele_wrapper->consumed;
    });
  }

  void BlockingGet(T &ele) {
    std::unique_lock lock(mutex_);
    cond_.wait(lock, [&](){
      return deque_.size() > 0;
    });
    auto ele_wrapper = *deque_.rbegin();
    deque_.pop_back();
    lock.unlock();
    cond_.notify_one();

    /// consume ele
    ele = std::move(ele_wrapper->ele);
    std::unique_lock ele_lock(ele_wrapper->mutex);
    ele_wrapper->consumed = true;
    ele_lock.unlock();
    ele_wrapper->cond.notify_one();
  }

 private:
  struct EleWrapper {
    std::mutex mutex;
    std::condition_variable cond;
    T ele;
    bool consumed = false;
  };

 private:
  std::mutex mutex_;
  std::condition_variable cond_;
  std::deque<std::shared_ptr<EleWrapper>> deque_;
};

}

#endif //PLAYER_SRC_MISC_BLOCKINGCHANNEL_H_
