//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "DemuxContext.h"
#include "core/PlayerContext.h"
#include "core/SyncContext.h"
#include "input/InputContext.h"
#include "demuxer/DemuxerFactory.h"

common::Error demux::DemuxContext::init(const core::player_ctx_sptr &player_ctx) {
  this->_sync_ctx = player_ctx->sync_ctx;
  this->_input_context = player_ctx->input_ctx;
  this->_running = true;
  this->_thread.run([&]() {
    do {} while (this->loop());
  });
  return common::Error::SUCCESS;
}

bool demux::DemuxContext::loop() {
  if (this->_demuxer == nullptr) {
    core::play_entry_sptr entry = nullptr;
    if (this->_input_context->pollEvent(input::event::entryAvailable)) {
      this->_input_context->getCurrentEntry(entry);
    }
    if (entry == nullptr) return _running;
    this->_demuxer = demuxer::DemuxerFactory::create("av"); /// @todo put in config
    misc::vector_sptr<stream::stream_sptr> streams = nullptr;
    this->_demuxer->open(entry, streams);
    for (auto &stream : *streams) {
      this->_sync_ctx->addStream(stream);
    }
    /// sync context to next version
    this->_sync_ctx->close();
    this->_sync_ctx->version++;
  }
  auto err = this->_demuxer->epoch();
  if (err == common::Error::eof) {
    this->_demuxer->close();
    this->_demuxer = nullptr;
  }
  return _running;
}

common::Error demux::DemuxContext::stopRunning() {
  this->_running = false;
  this->_thread.join();
  return common::Error::SUCCESS;
}
