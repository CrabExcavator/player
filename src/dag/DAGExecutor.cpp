//
// Created by CrabExcavator on 2020/12/27.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "DAGExecutor.h"

#include <utility>

namespace dag {

DAGExecutor::DAGExecutor():
root_(nullptr),
future_node_(nullptr)
{}

common::Error DAGExecutor::Init(dag_node_sptr root) {
  auto ret = common::Error::SUCCESS;
  
  root_ = std::move(root);
  return ret;
}

common::Error DAGExecutor::Run() {
  auto ret = root_->Run();
  return ret;
}

common::Error DAGExecutor::Close() {
  auto ret = common::Error::SUCCESS;
  return ret;
}

}
