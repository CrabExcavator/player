//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_VIDEOOUTPUT_H
#define PLAYER_VIDEOOUTPUT_H

#include <memory>

#include "input/InputContext.h"
#include "video/driver/Driver.h"

namespace video {

    class VideoOutput: public std::enable_shared_from_this<VideoOutput> {
    public:
        VideoOutput() = delete;
        explicit VideoOutput(input::input_ctx_sptr input_ctx);
        VideoOutput(const VideoOutput& rhs) = delete;
        VideoOutput(VideoOutput&& rhs) noexcept;
        VideoOutput& operator = (const VideoOutput& rhs) = delete;
        VideoOutput& operator = (VideoOutput&& rhs) noexcept;
        ~VideoOutput() = default;
        void init();
        input::input_ctx_sptr getInputCtx();
        void loop();

    public:
        int window_width;
        int window_height;

    private:
        input::input_ctx_sptr _input_ctx;
        driver::driver_uptr _driver;
    };

    using vo_sptr = std::shared_ptr<VideoOutput>;
}

#endif //PLAYER_VIDEOOUTPUT_H
