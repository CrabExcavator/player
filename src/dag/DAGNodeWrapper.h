//
// Created by CrabExcavator on 2021/1/1.
// Copyright (c) 2021 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_DAG_DAGNODEWRAPPER_H_
#define PLAYER_SRC_DAG_DAGNODEWRAPPER_H_

#include "DAGNode.h"
#include "DAGContext.h"

namespace dag {

class DAGNodeWrapper: public DAGNode {
 public:
  DAGNodeWrapper();

  common::Error Init(const dag_ctx_sptr &dag_context);

  common::Error Add(const dag_node_sptr &dag_node);

  common::Error Run() override;

};

}

#endif //PLAYER_SRC_DAG_DAGNODEWRAPPER_H_
