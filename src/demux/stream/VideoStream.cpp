//
// Created by CrabExcavator on 2020/11/5.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "VideoStream.h"
#include "misc/Chain.h"
#include "demux/filter/FillVideo.h"
#include "demux/filter/Blit.h"

namespace demux::stream {

    common::error VideoStream::init() {
        this->_frame_filter_chain = std::make_shared<misc::Chain<frame_sptr>>();
        this->_frame_filter_chain
        ->addLast(std::make_shared<filter::FillVideo>())
        ->addLast(std::make_shared<filter::Blit>())
        ;
        return common::error::success;
    }

}