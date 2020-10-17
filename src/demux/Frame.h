//
// Created by weihan on 2020/10/11.
//

#ifndef PLAYER_FRAME_H
#define PLAYER_FRAME_H

#include <memory>
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
        AVFrame* raw() {return this->_frame;}
        void fill(const demux::stream_sptr& stream);

    public:
        video::image_format imgfmt = video::image_format::unknown;
        uint8_t* pixels = nullptr;
        int pitch = 0;
        int height = 0;

    private:
        AVFrame* _frame;
    };

}

#endif //PLAYER_FRAME_H
