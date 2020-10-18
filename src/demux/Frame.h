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
        void fill(const demux::stream_sptr& stream);

    public:
        video::image_format imgfmt = video::image_format::unknown;
        uint8_t* pixels = nullptr;
        int pitch = 0;
        int height = 0;
        // is it the first of frame of a stream
        bool first = false;
        std::chrono::steady_clock::duration time_base;
        int64_t pts = 0;

    private:
        AVFrame* _frame;
    };

}

#endif //PLAYER_FRAME_H
