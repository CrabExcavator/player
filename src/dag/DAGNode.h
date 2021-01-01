//
// Created by CrabExcavator on 2021/1/1.
// Copyright (c) 2021 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_DAG_DAGNODE_H_
#define PLAYER_SRC_DAG_DAGNODE_H_

#include <list>

#include "IDAGNode.h"
#include "misc/typeptr.h"
#include "common/Error.h"

namespace dag {

class DAGNode : public IDAGNode {
 public:
  DAGNode();

  common::Error Run() override = 0;

 protected:
  dag_ctx_sptr dag_context_;

  dag_node_wptr father_;

  std::list<dag_node_sptr> sons_;
};

}

#endif //PLAYER_SRC_DAG_DAGNODE_H_
