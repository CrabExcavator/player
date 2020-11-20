//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <SDL2/SDL.h>
#include <glog/logging.h>
#include <map>

#include "DriverSDL.h"
#include "output/video/VideoOutput.h"
#include "demux/frame/IFrame.h"
#include "output/video/ImageFormat.h"
#include "input/InputContext.h"

namespace output::video::driver {

static const std::map<video::ImageFormat, SDL_PixelFormatEnum> formats = {
    {video::ImageFormat::yuv420p, SDL_PIXELFORMAT_IYUV}
};

static SDL_PixelFormatEnum getFormat(video::ImageFormat imgfmt) {
  if (!formats.contains(imgfmt)) {
    return SDL_PIXELFORMAT_UNKNOWN;
  }
  return formats.at(imgfmt);
}

DriverSDL::~DriverSDL() {
  _renderer = nullptr;
  _window = nullptr;
  SDL_Quit();
}

common::Error DriverSDL::init(vo_sptr vo) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return common::Error::videoDriverInitFail;
  }
  window_uptr window{
      SDL_CreateWindow("air", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       vo->window_width_, vo->window_height_, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN),
      SDL_DestroyWindow
  };
  if (window == nullptr) {
    return common::Error::videoDriverInitFail;
  }
  _window.swap(window);
  renderer_uptr renderer{
      SDL_CreateRenderer(_window.get(), -1,
                         SDL_RENDERER_ACCELERATED),
      SDL_DestroyRenderer
  };
  if (renderer == nullptr) {
    return common::Error::videoDriverInitFail;
  }
  _renderer.swap(renderer);
  reConfig(vo);
  return common::Error::SUCCESS;
}

common::Error DriverSDL::drawImage(vo_sptr vo) {
  SDL_RenderClear(_renderer.get());
  SDL_SetTextureBlendMode(_texture.get(), SDL_BLENDMODE_NONE);
  if (vo->frame_rendering_ != nullptr) {
    void *pixels = nullptr;
    int pitch = 0;
    SDL_LockTexture(_texture.get(), nullptr, &pixels, &pitch);
    misc::vector_sptr<common::Slice> data = nullptr;
    vo->frame_rendering_->GetData(data);

    /// @todo put in fmt translate func
    for (auto &aData : *data) {
      memcpy(pixels, aData.GetPtr(), aData.GetLength());
      pixels = (void*)((uint8_t*)pixels + aData.GetLength());
    }

    SDL_UnlockTexture(_texture.get());
    SDL_RenderCopy(_renderer.get(), _texture.get(), nullptr, nullptr);
    SDL_RenderPresent(_renderer.get());
  }
  return common::Error::SUCCESS;
}

common::Error DriverSDL::waitEvents(vo_sptr vo) {
  int timeout_ms = 100;
  SDL_Event ev;
  while (SDL_WaitEventTimeout(&ev, timeout_ms)) {
    timeout_ms = 0;
    auto input_ctx = vo->GetInputCtx();
    switch (ev.type) {
      case SDL_QUIT:input_ctx->PutEvent(input::Event::EXIT);
        break;
      default:break;
    }
  }
  return common::Error::SUCCESS;
}

common::Error DriverSDL::reConfig(vo_sptr vo) {
  auto texture_fmt = getFormat(vo->image_format_);
  SDL_SetRenderDrawColor(_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
  texture_uptr texture{
      SDL_CreateTexture(_renderer.get(), texture_fmt,
                        SDL_TEXTUREACCESS_STREAMING, vo->img_pitch_, vo->img_height_),
      SDL_DestroyTexture
  };
  _texture.swap(texture);
  return common::Error::SUCCESS;
}

}