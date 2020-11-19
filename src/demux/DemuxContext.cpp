//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "DemuxContext.h"
#include "player/PlayerContext.h"
#include "common/SyncContext.h"
#include "input/InputContext.h"
#include "demuxer/DemuxerFactory.h"

namespace demux {

common::Error DemuxContext::init(const input::input_ctx_sptr &input_ctx,
                                        const common::sync_ctx_sptr &sync_ctx) {
  this->_sync_ctx = sync_ctx;
  this->_input_context = input_ctx;
  this->_running = true;
  this->_thread.run([&]() {
    do {} while (this->loop());
  });
  return common::Error::SUCCESS;
}

common::Error DemuxContext::Run() {
  do {} while (loop());
  return common::Error::SUCCESS;
}

bool DemuxContext::loop() {
  if (this->_demuxer == nullptr) {
    player::play_entry_sptr entry = nullptr;
    if (this->_input_context->pollEvent(input::Event::entryAvailable)) {
      this->_input_context->getCurrentEntry(entry);
    }
    if (entry == nullptr) return _running;
    this->_demuxer = demuxer::DemuxerFactory::create("av"); /// @todo put in config
    misc::vector_sptr<stream::stream_sptr> streams = nullptr;
    this->_demuxer->Open(entry, streams);
    for (auto &stream : *streams) {
      this->_sync_ctx->addStream(stream);
    }
    /// sync context to next version
    this->_sync_ctx->close();
    this->_sync_ctx->version++;
  }
  auto err = this->_demuxer->Epoch();
  if (err == common::Error::eof) {
    this->_demuxer->Close();
    this->_demuxer = nullptr;
  }
  return _running;
}

common::Error DemuxContext::stopRunning() {
  this->_running = false;
  this->_thread.join();
  return common::Error::SUCCESS;
}

}