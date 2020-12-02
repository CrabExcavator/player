//
// Created by CrabExcavator on 2020/12/2.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_TOOL_SDL_SDLMANAGER_H_
#define PLAYER_SRC_TOOL_SDL_SDLMANAGER_H_

#include <memory>
#include <string>
#include <functional>
#include <SDL2/SDL.h>

#include "misc/GlobalInstance.h"
#include "common/Error.h"

namespace tool::sdl {

using window_uptr = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>;

using renderer_uptr = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>>;

using texture_uptr = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Texture *)>>;

class SDLManager : public GlobalInstance {
 public:
  static common::Error Init();
  static common::Error Destroy();
  /**
   * @brief create window
   * @param [in] window_name
   * @param [out] window
   * @return error code
   */
  static common::Error CreateWindow(const std::string &window_name,
                                    int window_width,
                                    int window_height,
                                    window_uptr &window);

  /**
   * @brief create renderer
   * @param [in] window
   * @return error code
   */
  static common::Error CreateRenderer(const window_uptr &window, renderer_uptr &renderer);
};

}

#endif //PLAYER_SRC_TOOL_SDL_SDLMANAGER_H_
