//
// Created by CrabExcavator on 2021/1/1.
// Copyright (c) 2021 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_DAG_DAGROOT_H_
#define PLAYER_SRC_DAG_DAGROOT_H_

#include <memory>
#include <list>

#include "misc/typeptr.h"
#include "misc/Latch.h"
#include "common/Error.h"

namespace dag {

class DAGRoot : public std::enable_shared_from_this<DAGRoot> {
 public:
  DAGRoot();

  common::Error Init(const dag_ctx_sptr &dag_context);

  common::Error Add(const dag_node_sptr& node);

  void Wait();

 private:
  dag_ctx_sptr dag_context_;

  misc::Latch latch_;

  std::list<dag_node_sptr> sons_;
};

}

#endif //PLAYER_SRC_DAG_DAGROOT_H_
