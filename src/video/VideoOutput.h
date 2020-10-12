//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_VIDEOOUTPUT_H
#define PLAYER_VIDEOOUTPUT_H

#include <memory>

#include "input/InputContext.h"
#include "video/driver/Driver.h"
#include "misc/Thread.h"
#include "misc/typeptr.h"

namespace video {

    class VideoOutput: public std::enable_shared_from_this<VideoOutput> {
    public:
        friend class core::PlayerContext;
        VideoOutput();
        VideoOutput(const VideoOutput& rhs) = delete;
        VideoOutput(VideoOutput&& rhs) = default;
        VideoOutput& operator = (const VideoOutput& rhs) = delete;
        VideoOutput& operator = (VideoOutput&& rhs) = delete;
        ~VideoOutput() = default;
        void init(const core::player_ctx_sptr& player_ctx);
        input::input_ctx_sptr getInputCtx();
        bool loop();

    public:
        int window_width = 1920;
        int window_height = 1080;

    private:
        input::input_ctx_sptr _input_ctx;
        driver::driver_uptr _driver;
        misc::Thread _thread;
        bool running = false;
    };

}

#endif //PLAYER_VIDEOOUTPUT_H
