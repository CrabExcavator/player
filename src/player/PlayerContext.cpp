//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <memory>
#include <glog/logging.h>

#include "PlayerContext.h"
#include "common/Config.h"
#include "input/InputContext.h"
#include "output/audio/AudioOutput.h"
#include "output/video/VideoOutput.h"
#include "demux/DemuxContext.h"
#include "common/SyncContext.h"
#include "misc/Chain.h"
#include "input/handler/Universal.h"

namespace player {

static std::string audio_sample = "sample.mp3";
static std::string video_sample = "small_bunny_1080p_60fps.mp4";

common::Error PlayerContext::Init() {
  auto ret = common::Error::SUCCESS;

  this->play_list_ = std::make_shared<PlayList>();
  this->play_list_->addLast(std::make_shared<player::PlayEntry>
                               (player::entry_type::file, audio_sample, 0));
  this->sync_ctx_ = std::make_shared<common::SyncContext>();
  this->input_ctx_ = std::make_shared<input::InputContext>();
  this->demux_ctx_ = std::make_shared<demux::DemuxContext>();
  this->ao_ = std::make_shared<audio::AudioOutput>();
  this->vo_ = std::make_shared<video::VideoOutput>();
  this->event_handler_ = std::make_shared<misc::Chain<input::input_ctx_sptr>>();
  this->event_handler_->addLast(std::make_shared<input::handler::Universal>());

  if (common::Error::SUCCESS != (ret = this->input_ctx_->Init())) {
    LOG(ERROR) << "init input context fail";
  } else if (common::Error::SUCCESS != (ret = this->sync_ctx_->Init())) {
    LOG(ERROR) << "init sync context fail";
  } else if (common::Error::SUCCESS != (ret = this->demux_ctx_->init(this->input_ctx_, this->sync_ctx_))) {
    LOG(ERROR) << "init demux context fail";
  } else if (common::Error::SUCCESS != (ret = this->ao_->Init(this->sync_ctx_))) {
    LOG(ERROR) << "init ao fail";
  } else if (common::Error::SUCCESS != (ret = this->vo_->Init(this->input_ctx_, this->sync_ctx_))) {
    LOG(ERROR) << "init vo fail";
  }

  return ret;
}

common::Error PlayerContext::Run() {
  this->runners.emplace_back(misc::Future::CreateFutureNode(this->demux_ctx_));
  this->runners.emplace_back(misc::Future::CreateFutureNode(this->ao_));
  this->runners.emplace_back(misc::Future::CreateFutureNode(this->vo_));
  do {} while (this->Loop());
  return common::Error::SUCCESS;
}

common::Error PlayerContext::Stop() {
  this->demux_ctx_->stopRunning();
  this->ao_->StopRunning();
  this->vo_->StopRunning();
  return common::Error::SUCCESS;
}

bool PlayerContext::Loop() {
  auto ret = common::Error::SUCCESS;

  /// some vo function must be called in main loop
  this->vo_->LoopInMainThread();

  if (common::Error::SUCCESS != (ret = this->input_ctx_->HandleEvent(this->event_handler_))) {
    if (ret != common::Error::exit) {
      LOG(WARNING) << "exit with err code " << static_cast<int64_t>(ret);
    }
  }

  return ret == common::Error::SUCCESS;
}

}
