//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_THREAD_H
#define PLAYER_THREAD_H

#include <string>
#include <memory>
#include <thread>
#include <glog/logging.h>

namespace misc {

enum class ThreadStatus {
  Stop,
  Run,
};

/**
 * @brief Thread
 */
class Thread {
#define MISC_THREAD_TT "Thread " << _thread_name << " "
 public:
  /**
   * @brief default
   */
  Thread() = default;

  /**
   * @brief delete
   * @param rhs
   */
  Thread(const Thread &rhs) = delete;

  /**
   * @brief default
   * @param rhs
   */
  Thread(Thread &&rhs) noexcept {
    this->_thread_name = std::move(rhs._thread_name);
    this->_thread = std::move(rhs._thread);
    this->status_ = rhs.status_;
  }

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  Thread &operator=(const Thread &rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  Thread &operator=(Thread &&rhs) = delete;

  /**
   * @brief default
   * @param thread_name
   */
  explicit Thread(std::string thread_name) : _thread_name(std::move(thread_name)) {
    //LOG(INFO) << MISC_THREAD_TT << "create";
    this->status_ = ThreadStatus::Stop;
  }

  /**
   * @brief default
   */
  ~Thread() {
    //LOG(INFO) << MISC_THREAD_TT << "deCreate";
  };

  /**
   * @brief start thread
   * @tparam Fp fun to exec
   * @tparam Args args to fun
   * @param callable
   * @param args
   */
  template<typename Fp, typename ...Args>
  void run(Fp &&callable, Args &&... args) {
    //LOG(INFO) << MISC_THREAD_TT << "start";
    this->status_ = ThreadStatus::Run;
    this->_thread = std::move(std::thread(std::forward<Fp, Args...>(callable, args...)));
  }

  /**
   * @brief join
   */
  void join() {
    this->_thread.join();
    this->status_ = ThreadStatus::Stop;
    //LOG(INFO) << MISC_THREAD_TT << "join";
  }

  ThreadStatus GetStatus() {
    return this->status_;
  }

 private:
  std::string _thread_name;

  std::thread _thread;

  ThreadStatus status_ = ThreadStatus::Stop;
};

}

#endif //PLAYER_THREAD_H
