//
// Created by CrabExcavator on 2020/11/14.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_THREADPOOL_H_
#define PLAYER_SRC_MISC_THREADPOOL_H_

#include <deque>
#include <mutex>
#include <condition_variable>

#include "typeptr.h"
#include "common/Error.h"
#include "Thread.h"
#include "util.h"

namespace misc {

template <size_t Size>
class ThreadPool {
 public:
  ThreadPool() {
    for (int i = 0 ; i < Size ; i++) {
      this->free_slots_.add(i);
    }
  }

  common::Error Dispatch(const runnable_sptr& runner) {
    std::unique_lock<std::mutex> lock(mutex_);
    this->condition_variable_.wait_for(lock, [&](){
      return !this->free_slots_.empty();
    });
    auto slot = this->free_slots_.front();
    this->free_slots_.pop_front();
    assert(this->threads_[slot].GetStatus() != ThreadStatus::Run);
    this->threads_[slot].Run([&]() {
      runner->Run();
    });
    this->threads_[slot].join();
    this->free_slots_.push_back(slot);
    lock.unlock();
    this->condition_variable_.notify_one();
  }

 private:
  std::deque<uint64_t> free_slots_;
  std::array<Thread, Size> threads_;
  std::mutex mutex_;
  std::condition_variable condition_variable_;
};

}

#endif //PLAYER_SRC_MISC_THREADPOOL_H_
