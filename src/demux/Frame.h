//
// Created by weihan on 2020/10/11.
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
        // is it the first of frame of a stream
        bool first = false;

        // sample format in audio frame
        audio::sample_format sample_fmt = audio::sample_format::unknown;

        // image format in image frame
        video::image_format img_fmt = video::image_format::unknown;

        // packed formats that can be direct used in driver
        uint8_t* pixels = nullptr;

        int pitch = 0;
        int height = 0;

        //
        std::chrono::steady_clock::duration time_base{};


        int64_t pts = 0;

    private:
        // raw frame in ffmpeg
        AVFrame* _frame;
    };

}

#endif //PLAYER_FRAME_H
