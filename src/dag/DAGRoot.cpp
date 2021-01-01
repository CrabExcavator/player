//
// Created by CrabExcavator on 2021/1/1.
// Copyright (c) 2021 Studio F.L.A. All rights reserved.
//

#include <glog/logging.h>

#include "DAGRoot.h"
#include "DAGContext.h"

namespace dag {

DAGRoot::DAGRoot():
dag_context_(nullptr),
latch_(),
sons_()
{}

common::Error DAGRoot::Init(const dag_ctx_sptr &dag_context) {
  auto ret = common::Error::SUCCESS;

  dag_context->SetRoot(shared_from_this());
  dag_context_ = dag_context;
  return ret;
}

common::Error DAGRoot::Add(const dag_node_sptr& node) {
  auto ret = common::Error::SUCCESS;

  sons_.emplace_back(node);
  latch_.Add(1);
  dag_context_->Dispatch(node, [&](){
    latch_.Done();
  });
  return ret;
}

void DAGRoot::Wait() {
  latch_.Wait();
}

}