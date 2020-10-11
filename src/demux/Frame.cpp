//
// Created by weihan on 2020/10/11.
//

#include "Frame.h"

namespace demux {

    Frame::Frame() {
        this->_frame = av_frame_alloc();
    }

    Frame::~Frame() {
        av_frame_free(&this->_frame);
    }

}
