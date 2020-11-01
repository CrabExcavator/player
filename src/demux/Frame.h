//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_FRAME_H
#define PLAYER_FRAME_H

#include <memory>
#include <chrono>
extern "C" {
#include <libavformat/avformat.h>
};

#include "audio/sample_format.h"
#include "video/image_format.h"
#include "misc/typeptr.h"

namespace demux {

    class Frame {
    public:
        Frame();
        ~Frame();
        Frame(const Frame& rhs) = delete;
        Frame(Frame&& rhs) = delete;
        Frame& operator = (const Frame& rhs) = delete;
        Frame& operator = (Frame&& rhs) = delete;
        // todo return raw pointer have safety problem here, fixit
        AVFrame* raw() {return this->_frame;}

    public:
        // is it the first frame of a stream
        bool first = false;

        // is it the last frame of a stream
        bool last = false;

        // sample format in audio frame
        audio::sample_format sample_fmt = audio::sample_format::unknown;
        int sample_size = 0;

        // num of channel
        int num_of_channel = 0;

        // num of sample
        int num_of_sample = 0;

        // sample rate
        int sample_rate = 0;

        // image format in image frame
        video::image_format img_fmt = video::image_format::unknown;

        // packed formats that can be direct used in driver
        uint8_t* pixels = nullptr;

        int pitch = 0;
        int height = 0;

        std::chrono::steady_clock::duration time_base{};

        int64_t pts = 0;

    private:
        // raw frame in ffmpeg
        AVFrame* _frame;
    };

}

#endif //PLAYER_FRAME_H
