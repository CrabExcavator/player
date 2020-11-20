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

namespace video::driver {

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
  this->_renderer = nullptr;
  this->_window = nullptr;
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
  this->_window.swap(window);
  renderer_uptr renderer{
      SDL_CreateRenderer(this->_window.get(), -1,
                         SDL_RENDERER_ACCELERATED),
      SDL_DestroyRenderer
  };
  if (renderer == nullptr) {
    return common::Error::videoDriverInitFail;
  }
  this->_renderer.swap(renderer);
  this->reConfig(vo);
  return common::Error::SUCCESS;
}

common::Error DriverSDL::drawImage(vo_sptr vo) {
  SDL_RenderClear(this->_renderer.get());
//  SDL_SetTextureBlendMode(this->_texture.get(), SDL_BLENDMODE_NONE);
//  if (vo->frame_rendering_ != nullptr) {
//    void *pixels = nullptr;
//    int pitch = 0;
//    SDL_LockTexture(this->_texture.get(), nullptr, &pixels, &pitch);
//    memcpy(pixels, vo->frame_rendering_->pixels,
//           vo->img_pitch_ * vo->img_height_ + vo->img_pitch_ * vo->img_height_ / 4 + vo->img_pitch_ * vo->img_height_ / 4);
//    SDL_UnlockTexture(this->_texture.get());
//
//    SDL_RenderCopy(this->_renderer.get(), this->_texture.get(), nullptr, nullptr);
//    SDL_RenderPresent(this->_renderer.get());
//  }
  return common::Error::SUCCESS;
}

common::Error DriverSDL::waitEvents(vo_sptr vo) {
  int timeout_ms = 100;
  SDL_Event ev;
  while (SDL_WaitEventTimeout(&ev, timeout_ms)) {
    timeout_ms = 0;
    auto input_ctx = vo->GetInputCtx();
//    switch (ev.type) {
//      case SDL_QUIT:input_ctx->receiveEvent(input::Event::exit);
//        break;
//      default:break;
//    }
  }
  return common::Error::SUCCESS;
}

common::Error DriverSDL::reConfig(vo_sptr vo) {
  auto texture_fmt = getFormat(vo->image_format_);
  SDL_SetRenderDrawColor(this->_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
  texture_uptr texture{
      SDL_CreateTexture(this->_renderer.get(), texture_fmt,
                        SDL_TEXTUREACCESS_STREAMING, vo->img_pitch_, vo->img_height_),
      SDL_DestroyTexture
  };
  this->_texture.swap(texture);
  return common::Error::SUCCESS;
}

}