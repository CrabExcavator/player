//
// Created by weihan on 2020/10/7.
//

#ifndef PLAYER_VIDEOOUTPUT_H
#define PLAYER_VIDEOOUTPUT_H

#include <memory>
#include <chrono>
#include <folly/MPMCQueue.h>

#include "image_format.h"
#include "video/driver/VideoDriver.h"
#include "misc/Thread.h"
#include "misc/typeptr.h"

namespace video {

    class VideoOutput: public std::enable_shared_from_this<VideoOutput> {
    public:
        VideoOutput();
        VideoOutput(const VideoOutput& rhs) = delete;
        VideoOutput(VideoOutput&& rhs) = default;
        VideoOutput& operator = (const VideoOutput& rhs) = delete;
        VideoOutput& operator = (VideoOutput&& rhs) = delete;
        ~VideoOutput() = default;
        void init(const core::player_ctx_sptr& player_ctx);
        input::input_ctx_sptr getInputCtx();
        void loopInMainThread();
        void stopRunning();

    public:
        image_format imgfmt = image_format::unknown;
        int window_width = 1920;
        int window_height = 1080;
        int img_pitch = 1920;
        int img_height = 1080;
        std::shared_ptr<folly::MPMCQueue<demux::frame_sptr>> queue;
        demux::frame_sptr frame_rendering = nullptr;
        bool need_reConfig = false;

    private:
        bool loop();

    private:
        bool _running = false;
        driver::video_driver_uptr _driver = nullptr;
        std::chrono::steady_clock::time_point _last_tick{};
        std::chrono::steady_clock::duration _time_base{};
        int64_t _last_pts = 0;
        demux::frame_sptr _frame = nullptr;
        input::input_ctx_sptr _input_ctx;
        misc::Thread _thread;
    };

}

#endif //PLAYER_VIDEOOUTPUT_H
