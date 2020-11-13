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
    this->ptr_ = rhs.ptr_;
    this->length_ = rhs.length_;
  }

  Slice &operator = (const Slice &rhs) = default;

  Slice &operator = (Slice &&rhs) noexcept {
    this->ptr_ = rhs.ptr_;
    this->length_ = rhs.length_;
    return *this;
  }

 private:
  uint8_t *ptr_;

  uint64_t length_;
};

}

#endif //PLAYER_SRC_COMMON_SLICE_H_
