//
// Created by CrabExcavator on 2020/12/27.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_DAG_IDAGNODE_H_
#define PLAYER_SRC_DAG_IDAGNODE_H_

#include "common/Error.h"
#include "misc/Runnable.h"

namespace dag {

class IDAGNode : public misc::Runnable {
 public:
  common::Error Run() override = 0;
};

}

#endif //PLAYER_SRC_DAG_IDAGNODE_H_
