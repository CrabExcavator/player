//
// Created by CrabExcavator on 2020/12/2.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <glog/logging.h>

#include "SDLManager.h"

namespace tool::sdl {

std::unique_ptr<SDLManager> &SDLManager::GetInstance() {
  static std::unique_ptr<SDLManager> sdl_manager {
    new SDLManager()
  };
  return sdl_manager;
}

SDLManager::SDLManager():
inited_(false) {}

common::Error SDLManager::Init() {
  auto ret = common::Error::SUCCESS;

  if (inited_) {
    ret = common::Error::INITED_TWICE;
  } else if (0 > SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    ret = common::Error::SDL_ERR_INIT;
  } else {
    inited_ = true;
  }
  return ret;
}

common::Error SDLManager::Destroy() const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    ret = common::Error::NOT_INITED;
  } else {
    SDL_Quit();
  }
  return ret;
}

common::Error SDLManager::CreateWindow(const std::string &window_name,
                                       int window_width,
                                       int window_height,
                                       window_uptr &window) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    LOG(WARNING) << "sdl not Init";
    ret = common::Error::NOT_INITED;
  } else if (nullptr != window) {
    LOG(WARNING) << "window should be null";
    ret = common::Error::INVALID_ARGS;
  } else {
    window_uptr _window{
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
  }
  return ret;
}

common::Error SDLManager::CreateRenderer(const window_uptr &window, renderer_uptr &renderer) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    LOG(WARNING) << "sdl not Init";
    ret = common::Error::NOT_INITED;
  } else if (nullptr != renderer) {
    LOG(WARNING) << "rednerer should be null";
    ret = common::Error::INVALID_ARGS;
  } else if (nullptr == window) {
      LOG(WARNING) << "window should not be null";
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

common::Error SDLManager::RenderClear(const renderer_uptr &renderer) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    LOG(WARNING) << "sdl not Init";
    ret = common::Error::NOT_INITED;
  } else {
    SDL_RenderClear(renderer.get());
  }
  return ret;
}

common::Error SDLManager::SetRenderDrawColor(const renderer_uptr &renderer,
                                             uint8_t r,
                                             uint8_t g,
                                             uint8_t b,
                                             uint8_t a) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    LOG(WARNING) << "sdl not Init";
    ret = common::Error::NOT_INITED;
  } else {
    SDL_SetRenderDrawColor(renderer.get(), r, g, b, a);
  }
  return ret;
}

common::Error SDLManager::CreateTexture(const renderer_uptr &renderer,
                                        SDL_PixelFormatEnum texture_fmt,
                                        int image_pitch,
                                        int image_height,
                                        texture_uptr &texture) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    ret = common::Error::NOT_INITED;
    LOG(WARNING) << "sdl not Init";
  } else if (nullptr != texture) {
    ret = common::Error::INVALID_ARGS;
    LOG(WARNING) << "texture should be null";
  } else if (nullptr == renderer) {
    ret = common::Error::INVALID_ARGS;
    LOG(WARNING) << "renderer should not be null";
  } else {
      texture_uptr _texture {
        SDL_CreateTexture(renderer.get(),
                          texture_fmt,
                          SDL_TEXTUREACCESS_STREAMING,
                          image_pitch,
                          image_height),
                          SDL_DestroyTexture
      };
      if (nullptr == _texture) {
        ret = common::Error::UNKNOWN_ERROR;
        LOG(WARNING) << "create texture fail";
      } else {
        texture.swap(_texture);
      }
  }
  return ret;
}

common::Error SDLManager::SetTextureBlendMode(const texture_uptr &texture, SDL_BlendMode blend_mode) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    ret = common::Error::NOT_INITED;
    LOG(WARNING) << "sdl not Init";
  } else {
    SDL_SetTextureBlendMode(texture.get(), blend_mode);
  }
  return ret;
}

common::Error SDLManager::LockTexture(const texture_uptr &texture,
                                      const SDL_Rect *rect,
                                      void *&pixels,
                                      int &pitch) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    ret = common::Error::NOT_INITED;
    LOG(WARNING) << "sdl not Init";
  } else {
    SDL_LockTexture(texture.get(), rect, &pixels, &pitch);
  }
  return ret;
}

common::Error SDLManager::UnlockTexture(const texture_uptr &texture) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    ret = common::Error::NOT_INITED;
    LOG(WARNING) << "sdl not Init";
  } else {
    SDL_UnlockTexture(texture.get());
  }
  return ret;
}

common::Error SDLManager::RenderCopy(const renderer_uptr &renderer, const texture_uptr &texture) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    ret = common::Error::NOT_INITED;
    LOG(WARNING) << "sdl not Init";
  } else {
    SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
  }
  return ret;
}

common::Error SDLManager::RenderPresent(const renderer_uptr &renderer) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    ret = common::Error::NOT_INITED;
    LOG(WARNING) << "sdl not Init";
  } else {
    SDL_RenderPresent(renderer.get());
  }
  return ret;
}

common::Error SDLManager::WaitEventTimeout(int timeout_ms, SDL_Event &event) const {
  auto ret = common::Error::SUCCESS;

  if (!inited_) {
    ret = common::Error::NOT_INITED;
    LOG(WARNING) << "sdl not Init";
  } else if (0 == SDL_WaitEventTimeout(&event, timeout_ms)) {
    ret = common::Error::TIME_OUT;
  }
  return ret;
}

}
