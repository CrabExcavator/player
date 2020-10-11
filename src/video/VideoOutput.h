//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_VIDEOOUTPUT_H
#define PLAYER_VIDEOOUTPUT_H

#include <memory>

#include "input/InputContext.h"
#include "video/driver/Driver.h"
#include "misc/Thread.h"

namespace core {
    class PlayerContext;
}

namespace video {

    class VideoOutput: public std::enable_shared_from_this<VideoOutput> {
    public:
        friend class core::PlayerContext;
        VideoOutput() = delete;
        explicit VideoOutput(const std::shared_ptr<core::PlayerContext>& player_ctx);
        VideoOutput(const VideoOutput& rhs) = delete;
        VideoOutput(VideoOutput&& rhs) = default;
        VideoOutput& operator = (const VideoOutput& rhs) = delete;
        VideoOutput& operator = (VideoOutput&& rhs) = delete;
        ~VideoOutput() = default;
        void init();
        input::input_ctx_sptr getInputCtx();
        bool loop();

    public:
        int window_width;
        int window_height;

    private:
        input::input_ctx_sptr _input_ctx;
        driver::driver_uptr _driver;
        misc::Thread _thread;
        bool running;
    };

    using vo_sptr = std::shared_ptr<VideoOutput>;
}

#endif //PLAYER_VIDEOOUTPUT_H
