//
// Created by CrabExcavator on 2021/1/1.
// Copyright (c) 2021 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_DAG_DAGNODEWRAPPER_H_
#define PLAYER_SRC_DAG_DAGNODEWRAPPER_H_

#include <list>

#include "IDAGNode.h"
#include "DAGContext.h"

namespace dag {

class DAGNodeWrapper: public IDAGNode {
 public:
  DAGNodeWrapper();

  common::Error Init(const dag_ctx_sptr &dag_context);

  common::Error Add(const dag_node_sptr &dag_node);

  common::Error Run() override;

 private:
  dag_ctx_sptr dag_context_;

  std::list<dag_node_sptr> sons_;
};

}

#endif //PLAYER_SRC_DAG_DAGNODEWRAPPER_H_
