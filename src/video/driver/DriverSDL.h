//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_DRIVERSDL_H
#define PLAYER_DRIVERSDL_H

#include <memory>
#include <functional>
#include <SDL2/SDL.h>

#include "Driver.h"
#include "misc/typeptr.h"

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
        using renderer_uptr = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer*)>>;
        using texture_uptr = std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture*)>>;
        window_uptr _window;
        renderer_uptr _renderer;
        texture_uptr _texture;
        int _width{640};
        int _height{480};

        demux::frame_sptr _frame;
    };

}

#endif //PLAYER_DRIVERSDL_H
