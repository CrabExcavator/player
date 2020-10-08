//
// Created by weihan on 2020/10/7.
//

#include <SDL2/SDL.h>
#include <glog/logging.h>

#include "DriverSDL.h"
#include "video/VideoOutput.h"

namespace video::driver {

    static void window_deleter(SDL_Window* window) {
        LOG(INFO) << "window deleter called";
        if (window != nullptr) {
            SDL_DestroyWindow(window);
        } else {
            LOG(WARNING) << "destroy null window";
        }
    }

    static void surface_deleter(SDL_Surface* surface) {
        LOG(INFO) << "surface deleter called";
        if (surface != nullptr) {
            SDL_FreeSurface(surface);
        } else {
            LOG(WARNING) << "destroy null surface";
        }
    }

    DriverSDL::~DriverSDL() {
        this->_screen_surface = nullptr;
        this->_image_surface = nullptr;
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
                    window_deleter
            };
            if (window == nullptr) {
                LOG(WARNING) << "Window could not be created! SDL_Error: " << SDL_GetError();
                success = false;
            } else {
                _window.swap(window);
                surface_uptr screen_surface{
                        SDL_GetWindowSurface(_window.get()),
                        surface_deleter
                };
                _screen_surface.swap(screen_surface);
            }
        }
        if (!success) {
            throw "init SDL error"; // todo modify exception
        }
    }

    void DriverSDL::drawImage(vo_sptr vo) {
        SDL_UpdateWindowSurface(this->_window.get());
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