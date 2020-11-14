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

namespace video::driver {

/**
 * @brief video driver of sdl
 */
class DriverSDL : public VideoDriver {
 public:
  /**
   * @brief default
   */
  DriverSDL() = default;

  /**
   * @brief clear SDL
   */
  ~DriverSDL() override;

  /**
   * @brief setNumOfStream SDL && create window && create texture
   * @param [in] vo
   * @return error code
   */
  common::Error init(vo_sptr vo) override;

  /**
   * @brief draw one image in each call
   * @param [in] vo
   * @return error code
   */
  common::Error drawImage(vo_sptr vo) override;

  /**
   * @brief listen for event, should called in main thread
   * @param [in] vo
   * @return error code
   */
  common::Error waitEvents(vo_sptr vo) override;

  /**
   * @brief 1. reConfig texture
   * @param [in] vo
   * @return error code
   */
  common::Error reConfig(vo_sptr vo) override;

 private:
  using window_uptr = std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>>;

  using renderer_uptr = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>>;

  using texture_uptr = std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture *)>>;

  /**
   * @brief pointer to window
   */
  window_uptr _window;

  /**
   * @brief pointer to renderer
   */
  renderer_uptr _renderer;

  /**
   * @brief pointer to texture
   */
  texture_uptr _texture;
};

}

#endif //PLAYER_DRIVERSDL_H
