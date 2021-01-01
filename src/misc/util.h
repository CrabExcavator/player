//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_MISC_H
#define PLAYER_MISC_H

#include <string>
#include <memory>
#include <sstream>

namespace misc {

/**
 * @brief defer in go
 */
class Defer {
 public:
  using defer_sptr = std::shared_ptr<Defer>;
  Defer() = delete;
  template<typename Func, typename... Args>
  explicit Defer(Func &&func, Args &&... args): defer_(nullptr, [&](Defer *_) {
    func(std::forward<Args>(args)...);
  }) {}
 private:
  defer_sptr defer_;
};

#define DEFER(func, args...) misc::Defer _(func, ##args)

/**
 * @brief cast one ele to string
 * @tparam T
 * @param head
 * @return
 */
template<typename T>
inline std::string to_string(T &&head) {
  std::stringstream ss;
  ss << head;
  return ss.str();
}

/**
 * @brief cast many ele to string
 * @tparam T
 * @tparam Args
 * @param head
 * @param args
 * @return
 */
template<typename T, typename... Args>
inline std::string to_string(T &&head, Args &&... args) {
  return to_string(std::forward<T>(head)) + to_string(std::forward<Args>(args)...);
}

}

#endif //PLAYER_MISC_H
