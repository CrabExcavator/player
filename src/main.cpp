//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <glog/logging.h>

#include "player/PlayerContext.h"
#include "misc/Init.h"

int main(int argc, char *argv[]) {
  auto ret = common::Error::SUCCESS;

  misc::Init _(argc, argv);
  auto player_context = std::make_shared<player::PlayerContext>();
  if (common::Error::SUCCESS != (ret = player_context->Init())) {
    LOG(INFO) << "player ctx Init fail";
  } else if (common::Error::SUCCESS != (ret = player_context->Run())) {
    LOG(INFO) << "player ctx run fail";
  } else if (common::Error::SUCCESS != (ret = player_context->Destroy())) {
    LOG(INFO) << "player ctx destroy fail";
  } else {
    LOG(INFO) << "return successfully";
  }
  return common::Error::SUCCESS == ret ? 0 : -1;
}
