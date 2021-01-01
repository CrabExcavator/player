//
// Created by CrabExcavator on 2021/1/1.
// Copyright (c) 2021 Studio F.L.A. All rights reserved.
//

#include "DAGNodeWrapper.h"
#include "misc/Latch.h"

namespace dag {

DAGNodeWrapper::DAGNodeWrapper():
DAGNode()
{}

common::Error DAGNodeWrapper::Init(const dag::dag_ctx_sptr& dag_context) {
  auto ret = common::Error::SUCCESS;

  dag_context_ = dag_context;
  return ret;
}

common::Error DAGNodeWrapper::Add(const dag_node_sptr &dag_node) {
  auto ret = common::Error::SUCCESS;

  sons_.emplace_back(dag_node);
  return ret;
}

common::Error DAGNodeWrapper::Run() {
  auto ret = common::Error::SUCCESS;
  misc::Latch latch;

  latch.Add(sons_.size());
  for (auto &son : sons_) {
    dag_context_->Dispatch(son, [&](){
      latch.Done();
    });
  }
  latch.Wait();
  return ret;
}

}
