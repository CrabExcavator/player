//
// Created by CrabExcavator on 2020/11/8.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "DemuxerFactory.h"
#include "FFDemuxer.h"

namespace demux::demuxer {

demuxer_sptr DemuxerFactory::create(const std::string &name) {
  return std::make_shared<FFDemuxer>();
}

}