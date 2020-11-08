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
            auto video_stream = std::make_shared<VideoStream>();
            video_stream->init();
            stream = video_stream;
            stream->setStream(av_stream);
            stream->op = core::output_port::video;
        } else if (av_stream->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            auto audio_stream = std::make_shared<AudioStream>();
            audio_stream->init();
            stream = audio_stream;
            stream->setStream(av_stream);
            stream->op = core::output_port::audio;
        }
        return stream;
    }

}
