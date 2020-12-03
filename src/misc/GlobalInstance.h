//
// Created by CrabExcavator on 2020/12/2.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_GLOBALINSTANCE_H_
#define PLAYER_SRC_MISC_GLOBALINSTANCE_H_

#include <memory>

namespace misc {

template<typename T>
class GlobalInstance {
 public:
  GlobalInstance() = default;
  virtual ~GlobalInstance() = default;
  GlobalInstance(const GlobalInstance<T> &rhs) = delete;
  GlobalInstance(GlobalInstance<T> &&rhs) = delete;
  GlobalInstance &operator = (const GlobalInstance<T> &rhs) = delete;
  GlobalInstance &operator = (GlobalInstance<T> &&rhs) = delete;
};

}

#endif //PLAYER_SRC_MISC_GLOBALINSTANCE_H_
