//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DRIVERSDL_H
#define PLAYER_DRIVERSDL_H

#include <memory>
#include <functional>
#include <SDL2/SDL.h>

#include "VideoDriver.h"
#include "misc/typeptr.h"
#include "tool/sdl/SDLManager.h"

namespace output::video::driver {

/**
 * @brief video driver of sdl
 */
class DriverSDL : public VideoDriver {
 public:
  /**
   * @brief default
   */
  DriverSDL();

  /**
   * @brief Clear SDL
   */
  ~DriverSDL() override;

  /**
   * @brief setNumOfStream SDL && create window && create texture
   * @param [in] vo
   * @return error code
   */
  common::Error Init(vo_sptr vo) override;

  /**
   * @brief draw one image in each call
   * @param [in] vo
   * @return error code
   */
  common::Error DrawImage(vo_sptr vo) override;

  /**
   * @brief listen for event, should called in main thread
   * @param [in] vo
   * @return error code
   */
  common::Error WaitEvents(vo_sptr vo) override;

  /**
   * @brief 1. ReConfig texture
   * @param [in] vo
   * @return error code
   */
  common::Error ReConfig(vo_sptr vo) override;

 private:
  tool::sdl::window_uptr window_;
  tool::sdl::renderer_uptr renderer_;
  tool::sdl::texture_uptr texture_;
};

}

#endif //PLAYER_DRIVERSDL_H
