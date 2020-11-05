//
// Created by CrabExcavator on 2020/11/5.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "StreamFactory.h"
#include "VideoStream.h"
#include "AudioStream.h"

namespace demux::stream {

    stream_sptr StreamFactory::create(const AVStream* av_stream) {
        stream_sptr stream = nullptr;
        if (av_stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            stream = std::make_shared<VideoStream>();
            stream->setStream(av_stream);
            stream->op = core::output_port::video;
        } else if (av_stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            stream = std::make_shared<AudioStream>();
            stream->setStream(av_stream);
            stream->op = core::output_port::audio;
        }
        return stream;
    }

}
