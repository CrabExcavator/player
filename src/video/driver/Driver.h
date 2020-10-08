//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_DRIVER_H
#define PLAYER_DRIVER_H

#include <memory>

namespace video {

    class VideoOutput;
    using vo_sptr = std::shared_ptr<VideoOutput>;

    namespace driver {

        class Driver {
        public:
            Driver() = default;

            virtual ~Driver() = 0;

            virtual void init(vo_sptr vo) = 0;

            virtual void drawImage(vo_sptr vo) = 0;

            virtual void waitEvents(vo_sptr vo) = 0;
        };

        using driver_uptr = std::unique_ptr<Driver>;

    }

}

#endif //PLAYER_DRIVER_H