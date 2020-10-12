//
// Created by weihan on 2020/10/11.
//

#ifndef PLAYER_FRAME_H
#define PLAYER_FRAME_H

#include <memory>
extern "C" {
#include <libavformat/avformat.h>
};

namespace demux {

    class Frame {
    public:
        Frame();
        ~Frame();
        Frame(const Frame& rhs) = delete;
        Frame(Frame&& rhs) = delete;
        Frame& operator = (const Frame& rhs) = delete;
        Frame& operator = (Frame&& rhs) = delete;
        AVFrame* get() {return this->_frame;}

    private:
        AVFrame* _frame;
    };

}

#endif //PLAYER_FRAME_H
