//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

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

DriverSDL::DriverSDL():
window_(nullptr),
renderer_(nullptr),
texture_(nullptr){}

DriverSDL::~DriverSDL() {
  renderer_ = nullptr;
  window_ = nullptr;
}

common::Error DriverSDL::Init(vo_sptr vo) {
  auto ret = common::Error::SUCCESS;
  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();

  if (common::Error::SUCCESS != (ret = sdl_manager->CreateWindow("air",
                                                                 vo->window_width_,
                                                                 vo->window_height_,
                                                                 window_))) {
    LOG(WARNING) << "create window fail";
  } else if (common::Error::SUCCESS != (ret = sdl_manager->CreateRenderer(window_,
                                                                          renderer_))) {
    LOG(WARNING) << "create renderer fail";
  } else if (common::Error::SUCCESS != (ret = ReConfig(vo))) {
    LOG(WARNING) << "re_config fail";
  }
  return ret;
}

common::Error DriverSDL::DrawImage(vo_sptr vo) {
  auto ret = common::Error::SUCCESS;
  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();
  void *pixels = nullptr;
  int pitch = 0;
  misc::vector_sptr<misc::Slice> data = nullptr;

  if (common::Error::SUCCESS != (ret = sdl_manager->RenderClear(renderer_))) {
    LOG(WARNING) << "render Clear fail";
  } else if (nullptr == vo->frame_rendering_) {
    // do nothing
  } else if (common::Error::SUCCESS !=
  (ret = sdl_manager->SetTextureBlendMode(texture_, SDL_BLENDMODE_NONE))) {
    LOG(WARNING) << "set texture blend mode fail";
  } else if (common::Error::SUCCESS !=
  (ret = sdl_manager->LockTexture(texture_, nullptr, pixels, pitch))) {
    LOG(WARNING) << "lock texture fail";
  } else if (common::Error::SUCCESS != vo->frame_rendering_->GetData(data)) {
    LOG(WARNING) << "get data from frame fail";
  } else {
    for (auto &ele : *data) {
      memcpy(pixels, ele.GetPtr(), ele.GetLength());
      pixels = (void*)((uint8_t*)pixels + ele.GetLength());
    }
    if (common::Error::SUCCESS != (ret = sdl_manager->UnlockTexture(texture_))) {
      LOG(WARNING) << "unlock texture fail";
    } else if (common::Error::SUCCESS != (ret = sdl_manager->RenderCopy(renderer_, texture_))) {
      LOG(WARNING) << "render copy fail";
    } else if (common::Error::SUCCESS != (ret = sdl_manager->RenderPresent(renderer_))) {
      LOG(WARNING) << "render present fail";
    }
  }
  return ret;
}

common::Error DriverSDL::WaitEvents(vo_sptr vo) {
  auto ret = common::Error::SUCCESS;
  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();
  int timeout_ms = 10;
  SDL_Event ev;

  while (common::Error::SUCCESS == (ret = sdl_manager->WaitEventTimeout(timeout_ms, ev))) {
    /// should not get event once
    timeout_ms = 0;
    auto input_ctx = vo->GetInputCtx();
    switch (ev.type) {
      case SDL_QUIT:input_ctx->PutEvent(input::Event::EXIT);
        break;
      default:break;
    }
  }
  return ret;
}

common::Error DriverSDL::ReConfig(vo_sptr vo) {
  auto ret = common::Error::SUCCESS;
  auto &sdl_manager = tool::sdl::SDLManager::GetInstance();
  auto texture_fmt = getFormat(vo->image_format_);
  texture_ = nullptr;

  if (common::Error::SUCCESS !=
      (ret = sdl_manager->SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF))) {
    LOG(WARNING) << "set render draw color fail";
  } else if (common::Error::SUCCESS !=(ret = sdl_manager->CreateTexture(renderer_,
                                                                        texture_fmt,
                                                                        vo->img_pitch_,
                                                                        vo->img_height_,
                                                                        texture_))) {
    LOG(WARNING) << "create texture fail";
  }
  return ret;
}

}