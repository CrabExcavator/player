//
// Created by CrabExcavator on 2020/12/3.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <glog/logging.h>

#include "GlobalInstanceManager.h"
#include "tool/sdl/SDLManager.h"

namespace common {

Error GlobalInstanceManager::Init() {
  auto ret = Error::SUCCESS;

  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();
  if (Error::SUCCESS != (ret =sdl_manager->Init())) {
    LOG(WARNING) << "init sdl fail";
  }
  return ret;
}

Error GlobalInstanceManager::Destroy() {
  auto ret = Error::SUCCESS;

  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();
  if (Error::SUCCESS != (ret = sdl_manager->Destroy())) {
    LOG(WARNING) << "destroy sdl fail";
  }
  return ret;
}

}
