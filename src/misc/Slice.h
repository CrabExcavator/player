//
// Created by CrabExcavator on 2020/11/12.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_COMMON_SLICE_H_
#define PLAYER_SRC_COMMON_SLICE_H_

#include <cstdint>

namespace common {

class Slice {
 public:
  Slice() = delete;

  Slice(uint8_t *ptr, uint64_t length):
  ptr_(ptr), length_(length) {}

  Slice(const Slice &rhs) = default;

  Slice(Slice &&rhs) noexcept {
    ptr_ = rhs.ptr_;
    length_ = rhs.length_;
  }

  Slice &operator = (const Slice &rhs) = default;

  Slice &operator = (Slice &&rhs) noexcept {
    ptr_ = rhs.ptr_;
    length_ = rhs.length_;
    return *this;
  }

  uint8_t *GetPtr() {
    return ptr_;
  }

  uint64_t GetLength() const {
    return length_;
  }

  common::Error Copy(uint8_t *buffer) {
    /// @todo error
    memcpy(buffer, ptr_, length_);
    return common::Error::SUCCESS;
  }

 private:
  uint8_t *ptr_;

  uint64_t length_;
};

}

#endif //PLAYER_SRC_COMMON_SLICE_H_
