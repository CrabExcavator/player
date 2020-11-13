//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <glog/logging.h>
#include <map>

#include "Frame.h"

namespace demux {

Frame::Frame() {
  this->_frame = av_frame_alloc();
}

Frame::~Frame() {
  av_frame_free(&this->_frame);
  this->_frame = nullptr;
  free(this->pixels);
}

}
