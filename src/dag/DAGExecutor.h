//
// Created by CrabExcavator on 2020/12/27.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_DAG_DAGEXECUTOR_H_
#define PLAYER_SRC_DAG_DAGEXECUTOR_H_

#include "misc/Future.h"
#include "misc/typeptr.h"
#include "IDAGNode.h"

namespace dag {

class DAGExecutor : public IDAGNode {
 public:
  DAGExecutor();

  common::Error Init(dag_node_sptr root);

  common::Error Run() override;

  common::Error Close() override;

 private:
  dag_node_sptr root_;
  misc::future_node_sptr future_node_;
};

}

#endif //PLAYER_SRC_DAG_DAGEXECUTOR_H_
