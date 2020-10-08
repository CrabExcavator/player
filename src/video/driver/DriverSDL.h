//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_DRIVERSDL_H
#define PLAYER_DRIVERSDL_H

#include <memory>
#include <SDL2/SDL.h>

#include "Driver.h"

namespace video::driver {

    class DriverSDL: public Driver {
    public:
        DriverSDL() = default;
        ~DriverSDL() override;
        void init(vo_sptr vo) override;
        void drawImage(vo_sptr vo) override;
        void waitEvents(vo_sptr vo) override;
    private:
        using window_uptr = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>;
        using surface_uptr = std::unique_ptr<SDL_Surface, std::function<void(SDL_Surface*)>>;
        window_uptr _window;
        surface_uptr _screen_surface;
        surface_uptr _image_surface;
        int _width{};
        int _height{};
    };

}

#endif //PLAYER_DRIVERSDL_H
