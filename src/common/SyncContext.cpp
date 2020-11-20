//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "SyncContext.h"
#include "misc/util.h"
#include "demux/stream/IStream.h"

namespace common {

SyncContext::SyncContext() : _size(0), _cnt(0), _close(true) {

}

common::Error SyncContext::Init() {
  this->_video_streams = std::make_shared<std::vector<demux::stream::stream_sptr>>();
  this->_audio_streams = std::make_shared<std::vector<demux::stream::stream_sptr>>();
  this->version = 0;
  return common::Error::SUCCESS;
}

void SyncContext::wait() {
  if (this->_close) return;
  std::unique_lock<std::mutex> lock(this->_mutex);
  this->_cnt++;
  this->_cond.wait(lock, [&]() {
    return (this->_cnt >= this->_size) || (this->_close);
  });
  lock.unlock();
  this->_cond.notify_one();
}

void SyncContext::setNumOfStream(int size) {
  this->_mutex.lock();
  DEFER([&]() { this->_mutex.unlock(); });
  this->_cnt = 0;
  this->_size = size;
  this->_close = false;
  this->version = 0;
}

void SyncContext::close() {
  this->_close = true;
  this->_cond.notify_all();
}

common::Error SyncContext::addVideoStream(const demux::stream::stream_sptr &stream) {
  this->_video_streams->emplace_back(stream);
  return common::Error::SUCCESS;
}

common::Error SyncContext::addAudioStream(const demux::stream::stream_sptr &stream) {
  this->_audio_streams->emplace_back(stream);
  return common::Error::SUCCESS;
}

common::Error SyncContext::addStream(const demux::stream::stream_sptr &stream) {
  if (stream->GetOutputPort() == output::OutputPort::audio) {
    return this->addAudioStream(stream);
  } else if (stream->GetOutputPort() == output::OutputPort::video) {
    return this->addVideoStream(stream);
  }
  return common::Error::UNKNOWN_ERROR;
}

common::Error SyncContext::getVideoStream(demux::stream::stream_sptr &stream) {
  assert(this->_video_streams->size() <= 1);
  if (this->_video_streams->empty()) {
    return common::Error::noStream;
  }
  stream = this->_video_streams->at(0);
  this->_video_streams->clear();
  return common::Error::SUCCESS;
}

common::Error SyncContext::getAudioStream(demux::stream::stream_sptr &stream) {
  assert(this->_audio_streams->size() <= 1);
  if (this->_audio_streams->empty()) {
    return common::Error::noStream;
  }
  stream = this->_audio_streams->at(0);
  this->_audio_streams->clear();
  return common::Error::SUCCESS;
}

}