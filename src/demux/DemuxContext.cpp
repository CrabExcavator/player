//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "DemuxContext.h"
#include "input/InputContext.h"
#include "demuxer/DemuxerFactory.h"
#include "common/Slots.h"
#include "common/Config.h"
#include "demux/stream/IStream.h"
#include "output/OutputPort.h"
#include "common/Signal.h"

namespace demux {

common::Error DemuxContext::Init(const input::input_ctx_sptr &input_ctx,
                                 const common::sync_ctx_sptr &sync_ctx) {
  auto ret = common::Error::SUCCESS;

  this->sync_ctx_ = sync_ctx;
  this->input_context_ = input_ctx;
  this->running_ = true;
  this->AdjustHZ(GET_CONFIG(default_ticker_hz));
  return ret;
}

common::Error DemuxContext::Run() {
  do {} while (Loop());
  return common::Error::SUCCESS;
}

bool DemuxContext::LoopImpl() {
  if (this->demuxer_ == nullptr) {
    auto entry = BLOCKING_GET_FROM_SLOT(ENTRY_SLOT);
    if (entry == nullptr) return running_;
    this->demuxer_ = demuxer::DemuxerFactory::create("av");
    misc::vector_sptr<stream::stream_sptr> streams = nullptr;
    this->demuxer_->Open(entry, streams);
    for (auto &stream : *streams) {
      if (output::OutputPort::audio == stream->GetOutputPort()) {
        BLOCKING_PUSH_TO_SLOT(AUDIO_OUTPUT_STREAM_SLOT, stream);
        BLOCKING_PUSH_TO_SLOT(AUDIO_OUTPUT_CTL_SLOT, common::Signal::NEXT_STREAM);
      }
    }
  }
  if (common::Error::SUCCESS != this->demuxer_->Epoch()) {
    this->demuxer_->Close();
    this->demuxer_ = nullptr;
  }
  return running_;
}

common::Error DemuxContext::Stop() {
  this->running_ = false;
  /// because GET_FROM_SLOT may be blocking
  BLOCKING_PUSH_TO_SLOT(ENTRY_SLOT, nullptr);
  return common::Error::SUCCESS;
}

}