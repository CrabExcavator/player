//
// Created by CrabExcavator on 2020/11/9.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "FFStream.h"
#include "common/Config.h"
#include "misc/Chain.h"
#include "demux/frame/FFFrame.h"

namespace demux::stream {

common::Error FFStream::Init(const AVStream *stream) {
  auto codecParm = stream->codecpar;
  if (codecParm->codec_type == AVMEDIA_TYPE_AUDIO) {
    op_ = output::OutputPort::AUDIO;
  } else if (codecParm->codec_type == AVMEDIA_TYPE_VIDEO) {
    op_ = output::OutputPort::VIDEO;
  }
  codec_ = avcodec_find_decoder(codecParm->codec_id);
  av_codec_ctx_uptr codec_ctx(avcodec_alloc_context3(codec_), avcodec_free_context_wrapper);
  codec_ctx_.swap(codec_ctx);
  if (avcodec_parameters_to_context(codec_ctx_.get(), codecParm) < 0) {
    LOG(ERROR) << "avcodec_parameters_to_context fail";
    return common::Error::UNKNOWN_ERROR;
  } else if (avcodec_open2(codec_ctx_.get(), codec_, nullptr) < 0) {
    LOG(ERROR) << "avcodec_open2 fail";
    return common::Error::UNKNOWN_ERROR;
  }
  codec_ctx_->channel_layout = av_get_default_channel_layout(codec_ctx_->channels);
  time_base_ = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::duration<double>(av_q2d(stream->time_base)));
  queue_ = std::make_shared<folly::MPMCQueue<frame::ffframe_sptr>>(GET_CONFIG(default_queue_size));
  return common::Error::SUCCESS;
}

common::Error FFStream::Read(frame::frame_sptr &frame) {
  auto ret = common::Error::SUCCESS;

  frame::ffframe_sptr frame_ = nullptr;
  ret = queue_->read(frame_) ? common::Error::SUCCESS : common::Error::UN_READ;
  frame = frame_;
  return ret;
}

common::Error FFStream::Close() {
  auto frame = std::make_shared<frame::FFFrame>();
  frame->Init(nullptr, false, true);
  queue_->blockingWrite(frame);
  return common::Error::SUCCESS;
}

common::Error FFStream::Feed(const av_packet_sptr &packet) {
  auto err = common::Error::SUCCESS;
  if (avcodec_send_packet(codec_ctx_.get(), packet.get()) < 0) {
    return common::Error::UNKNOWN_ERROR;
  }
  int av_err = 0;
  do {
    auto frame = std::make_shared<frame::FFFrame>();
    auto av_frame = av_frame_alloc();
    av_err = avcodec_receive_frame(codec_ctx_.get(), av_frame);
    if (av_err == AVERROR(EAGAIN) || av_err == AVERROR_EOF) {
      break;
    } else if (av_err < 0) {
      return common::Error::UNKNOWN_ERROR;
    }
    frame->Init(av_frame, first_, false);
    if (output::OutputPort::VIDEO == op_) {
      frame->GetImageFormat();
    } else if (output::OutputPort::AUDIO == op_) {
      frame->GetSampleFormat();
    }
    if (first_) {
      first_ = false;
    }
    queue_->blockingWrite(frame);
  } while (av_err >= 0);
  return err;
}

std::chrono::nanoseconds FFStream::GetTimeBase() {
  return time_base_;
}

output::OutputPort FFStream::GetOutputPort() {
  return op_;
}

}