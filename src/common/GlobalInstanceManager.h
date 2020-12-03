//
// Created by CrabExcavator on 2020/12/3.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_COMMON_GLOBALINSTANCEMANAGER_H_
#define PLAYER_SRC_COMMON_GLOBALINSTANCEMANAGER_H_

#include "Error.h"

namespace common {

class GlobalInstanceManager {
 public:
  static Error Init();
  static Error Destroy();
};

}

#endif //PLAYER_SRC_COMMON_GLOBALINSTANCEMANAGER_H_
