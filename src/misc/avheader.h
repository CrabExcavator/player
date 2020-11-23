//
// Created by CrabExcavator on 2020/11/9.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_AVHEADER_H
#define PLAYER_AVHEADER_H

#include <memory>
#include <functional>
extern "C" {
#include <libavcodec/codec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
}

using av_packet_sptr = std::shared_ptr<AVPacket>;

using av_codec_ctx_uptr = std::unique_ptr<AVCodecContext, std::function<void(AVCodecContext *)>>;

using swr_ctx_uptr = std::unique_ptr<SwrContext, std::function<void(SwrContext *)>>;

static void avcodec_free_context_wrapper(AVCodecContext *ctx) {
  avcodec_free_context(&ctx);
}

static void swr_free_wrapper(SwrContext *ctx) {
  swr_free(&ctx);
}

#endif //PLAYER_AVHEADER_H
