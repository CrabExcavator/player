//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <memory>

#include "player/PlayerContext.h"
#include "misc/Init.h"

namespace osal {

/**
 * @brief main func in osx platform
 * @param [in] argc pass from true main func
 * @param [in] argv pass from true main func
 * @return error code i guess
 */
int player_main(int argc, char *argv[]) {
  auto ret = common::Error::SUCCESS;

  misc::Init _(argc, argv);
  auto player_context = std::make_shared<player::PlayerContext>();
  if (common::Error::SUCCESS != (ret = player_context->Init())) {
    LOG(INFO) << "player ctx Init fail";
  } else if (common::Error::SUCCESS != (ret = player_context->Run())) {
    LOG(INFO) << "player ctx run fail";
  } else {
    LOG(INFO) << "success return";
  }
  
  return 0;
}

}