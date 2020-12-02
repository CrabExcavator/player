//
// Created by CrabExcavator on 2020/12/2.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <glog/logging.h>

#include "SDLManager.h"

namespace tool::sdl {

common::Error SDLManager::Init() {
  auto ret = common::Error::SUCCESS;

  if (0 > SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    ret = common::Error::SDL_ERR_INIT;
  }
  return ret;
}

common::Error SDLManager::Destroy() {
  auto ret = common::Error::SUCCESS;

  SDL_Quit();
  return ret;
}

common::Error SDLManager::CreateWindow(const std::string &window_name,
                                       int window_width,
                                       int window_height,
                                       window_uptr &window) {
  auto ret = common::Error::SUCCESS;
  assert(window == nullptr);

  window_uptr _window {
    SDL_CreateWindow(window_name.c_str(),
                     SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED,
                     window_width,
                     window_height,
                     SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN
                     ),
    SDL_DestroyWindow
  };
  if (nullptr == _window) {
    ret = common::Error::SDL_ERR_CREATE_WINDOW;
  } else {
    window.swap(_window);
  }
  return ret;
}

common::Error SDLManager::CreateRenderer(const window_uptr &window, renderer_uptr &renderer) {
  auto ret = common::Error::SUCCESS;
  assert(renderer == nullptr);

  if (nullptr == window) {
    LOG(WARNING) << "should pass no null window in CreateRenderer";
    ret = common::Error::INVALID_ARGS;
  } else {
    renderer_uptr _renderer {
      SDL_CreateRenderer(window.get(),
                         -1,
                         SDL_RENDERER_ACCELERATED),
      SDL_DestroyRenderer
    };
    if (nullptr == _renderer) {
      ret = common::Error::SDL_ERR_CREATE_RENDERER;
    } else {
      renderer.swap(_renderer);
    }
  }
  return ret;
}

}
