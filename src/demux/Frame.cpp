//
// Created by weihan on 2020/10/11.
//

#include <map>

#include "Frame.h"
#include "demux/Stream.h"
#include "video/image_format.h"

namespace demux {

    Frame::Frame() {
        this->_frame = av_frame_alloc();
    }

    Frame::~Frame() {
        av_frame_free(&this->_frame);
        free(this->pixels);
    }

}
