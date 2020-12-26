//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_BUFFER_H
#define PLAYER_BUFFER_H

#include <algorithm>
#include <glog/logging.h>
#include <mutex>
#include <condition_variable>
#include <cstdint>

#include "misc/util.h"
#include "typeptr.h"

namespace misc {

/**
 * @brief a templated data structure to buffer data
 *
 * @tparam T element of Buffer
 * @tparam Size size of Buffer
 */
template<typename T, size_t Size>
class Buffer {
 public:
  /**
   * @brief default
   */
  Buffer() = default;

  /**
   * @brief copy array
   * @param rhs
   */
  Buffer(const Buffer &rhs) {
    array_ = rhs.array_;
  }

  /**
   * @brief move array
   * @param rhs
   */
  Buffer(Buffer &&rhs) noexcept {
    array_ = std::move(rhs.array_);
  }

  /**
   * @brief copy array
   * @param rhs
   * @return
   */
  Buffer &operator=(const Buffer &rhs) {
    array_ = rhs.array_;
  }

  /**
   * @brief move array
   * @param rhs
   * @return
   */
  Buffer &operator=(Buffer &&rhs) noexcept {
    array_ = std::move(rhs.array_);
  }

  void Get(T *dst, int beginOfEle, int numOfEle) {
    GetImpl(dst, beginOfEle, numOfEle);
  }

  void Put(const T *src, int beginOfEle, int numOfEle) {
    PutImpl(src, beginOfEle, numOfEle);
  }

  /**
   * @brief Clear all ele
   */
  void Clear() {
    mutex_.lock();
    DEFER([&]() { mutex_.unlock(); });
    close_ = false;
    head_ = 0;
    tail_ = 0;
    buffered_ele_ = 0;
  }

  /**
   * @brief Close buffer and release all choke thread
   */
  void Close() {
    close_ = true;
    cond_.notify_all();
  }

 private:
  /**
   * @brief internal cond
   * @param numOfEle
   * @return
   */
  inline bool GetCond(int numOfEle) {
    return buffered_ele_ >= numOfEle;
  }

  /**
   * @brief internal cond
   * @param numOfEle
   * @return
   */
  inline bool PutCond(int numOfEle) {
    return numOfEle + buffered_ele_ <= Size;
  }

  /**
   * @brief get ele from array to dst
   * @param [out] dst pointer to dst
   * @param [in] beginOfEle begin of ele to Put
   * @param [in] numOfEle number of ele to Put
   */
  template<typename delegate>
  inline void GetImpl(T *dst, int beginOfEle, int numOfEle) {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [&](){
      return close_ || GetCond(numOfEle);
    });
    int nxt_head = (head_ + numOfEle) % Size;
    for (int i = 0; i < numOfEle; i++) {
      dst[beginOfEle + i] = array_[(head_ + i) % Size];
    }
    head_ = nxt_head;
    buffered_ele_ -= numOfEle;
    lock.unlock();
    cond_.notify_one();
  }

  inline void GetImpl(uint8_t *dst, int beginOfEle, int numOfEle) {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [&](){
      return close_ || GetCond(numOfEle);
    });
    int nxt_head = (head_ + numOfEle) % Size;
    if (nxt_head <= head_) {
      memcpy(dst + beginOfEle, array_.data() + head_, Size - head_);
      memcpy(dst + beginOfEle + Size - head_, array_.data(), nxt_head);
    } else {
      memcpy(dst + beginOfEle, array_.data() + head_, numOfEle);
    }
    head_ = nxt_head;
    buffered_ele_ -= numOfEle;
    lock.unlock();
    cond_.notify_one();
  }

  /**
   * @brief Put ele in array with copy fashion
   * @param [in] src head of src
   * @param [in] beginOfEle begin of ele to copy
   * @param [in] numOfEle number of ele to copy
   */
  template<typename delegate>
  inline void PutImpl(const T *src, int beginOfEle, int numOfEle) {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [&](){
      return close_ || PutCond(numOfEle);
    });
    int nxt_tail = (tail_ + numOfEle) % Size;
    for (int i = 0; i < numOfEle; i++) {
      array_[(tail_ + i) % Size] = src[beginOfEle + i];
    }
    tail_ = nxt_tail;
    buffered_ele_ += numOfEle;
    lock.unlock();
    cond_.notify_one();
  }

  inline void PutImpl(const uint8_t *src, int beginOfEle, int numOfEle) {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [&](){
      return close_ || PutCond(numOfEle);
    });
    int nxt_tail = (tail_ + numOfEle) % Size;
    if (nxt_tail <= tail_) {
      memcpy(array_.data() + tail_, src + beginOfEle, Size - tail_);
      memcpy(array_.data(), src + beginOfEle + Size - tail_, nxt_tail);
    } else {
      memcpy(array_.data() + tail_, src + beginOfEle, numOfEle);
    }
    tail_ = nxt_tail;
    buffered_ele_ += numOfEle;
    lock.unlock();
    cond_.notify_one();
  }

 private:
  /**
   * @brief is buffer Close
   */
  std::atomic<bool> close_ = false;

  /**
   * @brief mutex
   */
  std::mutex mutex_;

  /**
   * @brief cond
   */
  std::condition_variable cond_;

  /**
   * @brief head of ele
   */
  int head_ = 0;

  /**
   * @brief tail of ele
   */
  int tail_ = 0;

  /**
   * @brief size of buffered ele
   */
  int buffered_ele_ = 0;

  /**
   * @brief ele array
   */
  std::array<T, Size> array_;
};

}

#endif //PLAYER_BUFFER_H
