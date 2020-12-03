//
// Created by CrabExcavator on 2020/12/3.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <gtest/gtest.h>

#include "tool/sdl/SDLManager.h"

TEST(SDL, GET_INSTANCE) {
  auto& sdl_manager = tool::sdl::SDLManager::GetInstance();
  ASSERT_EQ(common::Error::SUCCESS, sdl_manager->Init());
  auto& another_sdl_manager = tool::sdl::SDLManager::GetInstance();
  ASSERT_NE(common::Error::SUCCESS, another_sdl_manager->Init());
  ASSERT_EQ(common::Error::SUCCESS, sdl_manager->Destroy());
}
