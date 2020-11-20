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
#include <chrono>

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
    _array = rhs._array;
  }

  /**
   * @brief move array
   * @param rhs
   */
  Buffer(Buffer &&rhs) noexcept {
    _array = std::move(rhs._array);
  }

  /**
   * @brief copy array
   * @param rhs
   * @return
   */
  Buffer &operator=(const Buffer &rhs) {
    _array = rhs._array;
  }

  /**
   * @brief move array
   * @param rhs
   * @return
   */
  Buffer &operator=(Buffer &&rhs) noexcept {
    _array = std::move(rhs._array);
  }

  /**
   * @brief put ele in array with copy fashion
   * @param [in] src head of src
   * @param [in] beginOfEle begin of ele to copy
   * @param [in] numOfEle number of ele to copy
   */
  template<typename INT1, typename INT2>
  void put(const T *src, INT1 beginOfEle, INT2 numOfEle) {
    std::unique_lock<std::mutex> lock(_mutex);
    while (!_close) {
      auto flag =
          _cond.wait_for(lock, std::chrono::milliseconds(500), [&]() { return _put_cond(numOfEle); });
      if (_close) return;
      if (flag) {
        break;
      } else {
        continue;
      }
    }
    int nxt_tail = _tail + numOfEle;
    for (int i = 0; i < numOfEle; i++) {
      _array[(_tail + i) % Size] = src[beginOfEle + i];
    }
    _tail = nxt_tail % Size;
    _buffered_ele += numOfEle;
    lock.unlock();
    _cond.notify_one();
  }

  /**
   * @brief put ele in array with copy fashion
   * @tparam oSize the size of src array
   * @param [in] src src array to copy
   * @param [in] beginOfEle begin of ele to copy
   * @param [in] numOfEle number of ele to copy
   */
  template<typename INT1, typename INT2, size_t oSize>
  void put(const std::array<T, oSize> &src, INT1 beginOfEle, INT2 numOfEle) {
    std::unique_lock<std::mutex> lock(_mutex);
    while (!_close) {
      auto flag =
          _cond.wait_for(lock, std::chrono::milliseconds(500), [&]() { return _put_cond(numOfEle); });
      if (_close) return;
      if (flag) {
        break;
      } else {
        continue;
      }
    }
    int nxt_tail = _tail + numOfEle;
    for (int i = 0; i < numOfEle; i++) {
      _array[(_tail + i) % Size] = src[(beginOfEle + i) % oSize];
    }
    _tail = nxt_tail % Size;
    _buffered_ele += numOfEle;
    lock.unlock();
    _cond.notify_one();
  }

  /**
   * @brief get ele from array to dst
   * @param [out] dst pointer to dst
   * @param [in] beginOfEle begin of ele to put
   * @param [in] numOfEle number of ele to put
   */
  template<typename INT1, typename INT2>
  void get(T *dst, INT1 beginOfEle, INT2 numOfEle) {
    std::unique_lock<std::mutex> lock(_mutex);
    while (!_close) {
      auto flag =
          _cond.wait_for(lock, std::chrono::milliseconds(500), [&]() { return _get_cond(numOfEle); });
      if (_close) return;
      if (flag) {
        break;
      } else {
        continue;
      }
    }
    int nxt_head = _head + numOfEle;
    for (int i = 0; i < numOfEle; i++) {
      dst[beginOfEle + i] = _array[(_head + i) % Size];
    }
    _head = nxt_head % Size;
    _buffered_ele -= numOfEle;
    lock.unlock();
    _cond.notify_one();
  }

  /**
   * @brief get ele from array to dst
   * @tparam oSize size of dst array
   * @param [out] dst pointer to dst
   * @param [in] beginOfEle begin of ele to put
   * @param [in] numOfEle number of ele to put
   */
  template<typename INT1, typename INT2, size_t oSize>
  void get(std::array<T, oSize> &dst, INT1 beginOfEle, INT2 numOfEle) {
    std::unique_lock<std::mutex> lock(_mutex);
    while (!_close) {
      auto flag =
          _cond.wait_for(lock, std::chrono::milliseconds(500), [&]() { return _get_cond(numOfEle); });
      if (_close) return;
      if (flag) {
        break;
      } else {
        continue;
      }
    }
    int nxt_head = _head + numOfEle;
    for (int i = 0; i < numOfEle; i++) {
      dst[(beginOfEle + i) % oSize] = _array[(_head + i) % Size];
    }
    _head = nxt_head % Size;
    _buffered_ele -= numOfEle;
    lock.unlock();
    _cond.notify_one();
  }

  /**
   * @brief clear all ele
   */
  void clear() {
    _mutex.lock();
    DEFER([&]() { _mutex.unlock(); });
    _close = false;
    _head = 0;
    _tail = 0;
    _buffered_ele = 0;
  }

  /**
   * @brief close buffer and release all choke thread
   */
  void close() {
    _close = true;
  }

  /**
   * @brief size of buffer
   * @return size of buffer
   */
  int size() {
    return _buffered_ele;
  }

 private:
  /**
   * @brief internal cond
   * @param numOfEle
   * @return
   */
  inline bool _get_cond(int numOfEle) {
    return _buffered_ele >= numOfEle;
  }

  /**
   * @brief internal cond
   * @param numOfEle
   * @return
   */
  inline bool _put_cond(int numOfEle) {
    return numOfEle + _buffered_ele <= Size;
  }

 private:
  /**
   * @brief is buffer close
   */
  std::atomic<bool> _close = false;

  /**
   * @brief mutex
   */
  std::mutex _mutex;

  /**
   * @brief cond
   */
  std::condition_variable _cond;

  /**
   * @brief head of ele
   */
  int _head = 0;

  /**
   * @brief tail of ele
   */
  int _tail = 0;

  /**
   * @brief size of buffered ele
   */
  int _buffered_ele = 0;

  /**
   * @brief ele array
   */
  std::array<T, Size> _array;
};

}

#endif //PLAYER_BUFFER_H
