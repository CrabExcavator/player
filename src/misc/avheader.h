//
// Created by CrabExcavator on 2020/11/9.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_AVHEADER_H
#define PLAYER_AVHEADER_H

#include <memory>
extern "C" {
#include <libavcodec/codec.h>
#include <libavformat/avformat.h>
}

using av_packet_sptr = std::shared_ptr<AVPacket>;

using av_codec_ctx_uptr = std::unique_ptr<AVCodecContext, std::function<void(AVCodecContext*)>>;

#endif //PLAYER_AVHEADER_H
