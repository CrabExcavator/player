//
// Created by weihan on 2020/10/7.
//

#include <SDL2/SDL.h>
#include <glog/logging.h>

#include "DriverSDL.h"
#include "video/VideoOutput.h"

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
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            LOG(WARNING) << "SDL could not initialize! SDL_Error: " << SDL_GetError();
            success = false;
        } else {
            window_uptr window{
                    SDL_CreateWindow("air", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                     this->_width, this->_height, SDL_WINDOW_SHOWN),
                    SDL_DestroyWindow
            };
            if (window == nullptr) {
                LOG(WARNING) << "Window could not be created! SDL_Error: " << SDL_GetError();
                success = false;
            } else {
                this->_window.swap(window);
                renderer_uptr renderer{
                    SDL_CreateRenderer(this->_window.get(), -1, SDL_RENDERER_ACCELERATED), // todo choose best driver
                    SDL_DestroyRenderer
                };
                if (renderer == nullptr) {
                    LOG(WARNING) << "Renderer could not be created! SDL_Error: " << SDL_GetError();
                    success = false;
                }
                this->_renderer.swap(renderer);
                SDL_SetRenderDrawColor(this->_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
        if (!success) {
            throw "init SDL error"; // todo modify exception
        }
    }

    void DriverSDL::drawImage(vo_sptr vo) {
        SDL_RenderClear(this->_renderer.get());
        // todo render picture here
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