//
// Created by CrabExcavator on 2021/1/1.
// Copyright (c) 2021 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_DAG_DAGCONTEXT_H_
#define PLAYER_SRC_DAG_DAGCONTEXT_H_

#include "common/Error.h"
#include "IDAGNode.h"
#include "misc/typeptr.h"
#include "misc/Future.h"

namespace dag {

class DAGContext {
 public:
  DAGContext();

  template<typename Callback, typename... Args>
  common::Error Dispatch(const dag_node_sptr &node, Callback &&callback, Args &&...args) {
    auto ret = common::Error::SUCCESS;

    misc::Future::Dispatch(node, std::forward<Callback>(callback), std::forward<Args>(args)...);
    return ret;
  }
};

}

#endif //PLAYER_SRC_DAG_DAGCONTEXT_H_
