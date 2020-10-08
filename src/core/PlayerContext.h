//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_PLAYERCONTEXT_H
#define PLAYER_PLAYERCONTEXT_H

#include <memory>

namespace core {

    class PlayerContextImpl;

    class PlayerContext {
    public:
        PlayerContext();
        PlayerContext(const PlayerContext& rhs) = delete;
        PlayerContext(PlayerContext&& rhs) noexcept;
        PlayerContext& operator = (const PlayerContext& rhs) = delete;
        PlayerContext& operator = (PlayerContext&& rhs) noexcept;
        ~PlayerContext();

        void run();
    private:
        std::unique_ptr<PlayerContextImpl> _impl;
    };

}

#endif //PLAYER_PLAYERCONTEXT_H
