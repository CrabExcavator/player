//
// Created by weihan on 2020/10/7.
//

#include <SDL2/SDL.h>
#include <glog/logging.h>

#include "DriverSDL.h"
#include "video/VideoOutput.h"
#include "exception/InitException.h"
#include "demux/Frame.h"

namespace video::driver {

    DriverSDL::~DriverSDL() {
        this->_renderer = nullptr;
        this->_window = nullptr;
        SDL_Quit();
    }

    void DriverSDL::init(vo_sptr vo) {
        this->_width = vo->window_width;
        this->_height = vo->window_height;
        bool success = true;
        do {
            if (SDL_Init(SDL_INIT_VIDEO) < 0) {
                LOG(WARNING) << "SDL could not initialize! SDL_Error: " << SDL_GetError();
                success = false;
                break;
            } else {
                window_uptr window{
                        SDL_CreateWindow("air", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                         this->_width, this->_height, SDL_WINDOW_SHOWN),
                        SDL_DestroyWindow
                };
                if (window == nullptr) {
                    LOG(WARNING) << "Window could not be created! SDL_Error: " << SDL_GetError();
                    success = false;
                    break;
                } else {
                    this->_window.swap(window);
                    renderer_uptr renderer{
                            SDL_CreateRenderer(this->_window.get(), -1,
                                               SDL_RENDERER_ACCELERATED), // todo choose best driver
                            SDL_DestroyRenderer
                    };
                    if (renderer == nullptr) {
                        LOG(WARNING) << "Renderer could not be created! SDL_Error: " << SDL_GetError();
                        success = false;
                        break;
                    }
                    this->_renderer.swap(renderer);
                    SDL_SetRenderDrawColor(this->_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
                    texture_uptr texture{
                            SDL_CreateTexture(this->_renderer.get(), SDL_PIXELFORMAT_IYUV,
                                              SDL_TEXTUREACCESS_STREAMING, this->_width, this->_height),
                            SDL_DestroyTexture
                    };
                    if (texture == nullptr) {
                        LOG(WARNING) << "texture could not be created! SDL_Error: " << SDL_GetError();
                        success = false;
                        break;
                    }
                    this->_texture.swap(texture);
                }
            }
        } while(false);
        if (!success) {
            throw exception::SDLInitException();
        }
    }

    void DriverSDL::drawImage(vo_sptr vo) {
        SDL_RenderClear(this->_renderer.get());
        SDL_SetTextureBlendMode(this->_texture.get(), SDL_BLENDMODE_NONE);
        if (vo->queue->read(this->_frame)) {
            auto frame = this->_frame->get();
            //SDL_UpdateYUVTexture(this->_texture.get(), nullptr, frame->data[0], frame->linesize[0],
            //                     frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);
            void* pixels = nullptr;
            int pitch = 0;
            SDL_LockTexture(this->_texture.get(), nullptr, &pixels, &pitch);
            memcpy(pixels, frame->data[0], _width * _height);
            pixels = (uint8_t*)pixels + _width * _height;
            memcpy(pixels, frame->data[1], _width * _height / 4);
            pixels = (uint8_t*)pixels + _width * _height / 4;
            memcpy(pixels, frame->data[2], _width * _height / 4);
            SDL_UnlockTexture(this->_texture.get());

            SDL_RenderCopy(this->_renderer.get(), this->_texture.get(), nullptr, nullptr);
        }
        SDL_RenderPresent(this->_renderer.get());
    }

    void DriverSDL::waitEvents(vo_sptr vo) {
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
                default:
                    break;
            }
        }
    }

}