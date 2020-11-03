//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <SDL2/SDL.h>
#include <glog/logging.h>
#include <map>

#include "DriverSDL.h"
#include "video/VideoOutput.h"
#include "exception/InitException.h"
#include "demux/Frame.h"
#include "video/image_format.h"
#include "input/InputContext.h"

namespace video::driver {

    static const std::map<video::image_format, SDL_PixelFormatEnum> formats = {
            {video::image_format::yuv420p, SDL_PIXELFORMAT_IYUV}
    };

    static SDL_PixelFormatEnum getFormat(video::image_format imgfmt) {
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

    common::error DriverSDL::init(vo_sptr vo) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            return common::error::videoDriverInitFail;
        }
        window_uptr window{
                SDL_CreateWindow("air", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                 vo->window_width, vo->window_height,  SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN),
                SDL_DestroyWindow
        };
        if (window == nullptr) {
            return common::error::videoDriverInitFail;
        }
        this->_window.swap(window);
        renderer_uptr renderer{
                SDL_CreateRenderer(this->_window.get(), -1,
                                   SDL_RENDERER_ACCELERATED),
                SDL_DestroyRenderer
        };
        if (renderer == nullptr) {
            return common::error::videoDriverInitFail;
        }
        this->_renderer.swap(renderer);
        this->reConfig(vo);
        return common::error::success;
    }

    common::error DriverSDL::drawImage(vo_sptr vo) {
        SDL_RenderClear(this->_renderer.get());
        SDL_SetTextureBlendMode(this->_texture.get(), SDL_BLENDMODE_NONE);
        if (vo->frame_rendering != nullptr) {
            void* pixels = nullptr;
            int pitch = 0;
            SDL_LockTexture(this->_texture.get(), nullptr, &pixels, &pitch);
            memcpy(pixels, vo->frame_rendering->pixels,
                   vo->img_pitch * vo->img_height + vo->img_pitch * vo->img_height / 4 + vo->img_pitch * vo->img_height / 4);
            SDL_UnlockTexture(this->_texture.get());

            SDL_RenderCopy(this->_renderer.get(), this->_texture.get(), nullptr, nullptr);
            SDL_RenderPresent(this->_renderer.get());
        }
        return common::error::success;
    }

    common::error DriverSDL::waitEvents(vo_sptr vo) {
        int timeout_ms = 100;
        SDL_Event ev;
        while (SDL_WaitEventTimeout(&ev, timeout_ms)) {
            timeout_ms = 0;
            auto input_ctx = vo->getInputCtx();
            switch (ev.type) {
                case SDL_QUIT:
                    input_ctx->receive(input::event::exit);
                    break;
                case SDL_KEYDOWN: {
                    switch (ev.key.keysym.sym) {
                        case SDLK_UP:
                            input_ctx->receive(input::event::key_up);
                            break;
                        case SDLK_DOWN:
                            input_ctx->receive(input::event::key_down);
                            break;
                        case SDLK_LEFT:
                            input_ctx->receive(input::event::key_left);
                            break;
                        case SDLK_RIGHT:
                            input_ctx->receive(input::event::key_right);
                            break;
                        default:
                            break;
                    }
                }
                case SDL_WINDOWEVENT: {
                    switch (ev.window.event) {
                        case SDL_WINDOWEVENT_SIZE_CHANGED:
                            int w, h;
                            SDL_GetWindowSize(this->_window.get(), &w, &h);
                            vo->window_width = w;
                            vo->window_height = h;
                            input_ctx->receive(input::event::window_resize);
                            break;
                        default:
                            break;
                    }
                }
                default:
                    break;
            }
        }
        return common::error::success;
    }

    common::error DriverSDL::reConfig(vo_sptr vo) {
        auto texture_fmt = getFormat(vo->imgfmt);
        SDL_SetRenderDrawColor(this->_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        texture_uptr texture{
                SDL_CreateTexture(this->_renderer.get(), texture_fmt,
                                  SDL_TEXTUREACCESS_STREAMING, vo->img_pitch, vo->img_height),
                SDL_DestroyTexture
        };
        this->_texture.swap(texture);
        return common::error::success;
    }

}