//
// Created by CrabExcavator on 2020/11/19.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_FUTURE_H_
#define PLAYER_SRC_MISC_FUTURE_H_

#include <thread>
#include <memory>
#include <utility>

#include "common/Error.h"
#include "misc/Runnable.h"
#include "misc/typeptr.h"

namespace misc {

class Future;

class FutureNode;

using future_node_sptr = std::shared_ptr<FutureNode>;

class FutureNode {
 public:
  friend class Future;

 public:
  common::Error Join() {
    thread_.join();
    return error_;
  }

 private:
  FutureNode() = default;

  static future_node_sptr Create() {
    auto future_node = new FutureNode();
    return std::shared_ptr<FutureNode>(future_node);
  }

  common::Error Init(misc::runnable_sptr runner) {
    runner_ = std::move(runner);
    thread_ = std::thread([&](){
      error_ = runner_->Run();
    });
    return common::Error::SUCCESS;
  }

 private:
  misc::runnable_sptr runner_;

  std::thread thread_;

  common::Error error_ = common::Error::SUCCESS;
};

class Future {
 public:
  static future_node_sptr CreateFutureNode(misc::runnable_sptr runner) {
    auto future_node = FutureNode::Create();
    future_node->Init(std::move(runner));
    return future_node;
  }
};

}

#endif //PLAYER_SRC_MISC_FUTURE_H_
