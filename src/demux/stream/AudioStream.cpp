//
// Created by CrabExcavator on 2020/11/5.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "AudioStream.h"
#include "misc/Chain.h"
#include "demux/filter/FillAudio.h"

namespace demux::stream {

    common::error AudioStream::init() {
        this->_frame_filter_chain = std::make_shared<misc::Chain<frame_sptr>>();
        this->_frame_filter_chain
        ->addLast(std::make_shared<filter::FillAudio>())
        ;
        return common::error::success;
    }

}