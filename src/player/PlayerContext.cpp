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
#include "misc/Chain.h"
#include "input/handler/Universal.h"
#include "common/Slots.h"

namespace player {

static std::string audio_sample = "sample.mp3";
static std::string video_sample = "small_bunny_1080p_60fps.mp4";

common::Error PlayerContext::Init() {
  auto ret = common::Error::SUCCESS;

  input_ctx_ = std::make_shared<input::InputContext>();
  demux_ctx_ = std::make_shared<demux::DemuxContext>();
  ao_ = std::make_shared<output::audio::AudioOutput>();
  vo_ = std::make_shared<output::video::VideoOutput>();
  event_handler_ = std::make_shared<misc::Chain<input::input_ctx_sptr>>();
  auto universal_handler = std::make_shared<input::handler::Universal>();
  universal_handler->Init(shared_from_this());
  event_handler_->AddLast(universal_handler);

  if (common::Error::SUCCESS != (ret = input_ctx_->Init())) {
    LOG(ERROR) << "Init input context fail";
  } else if (common::Error::SUCCESS != (ret = demux_ctx_->Init(input_ctx_))) {
    LOG(ERROR) << "Init demux context fail";
  } else if (common::Error::SUCCESS != (ret = ao_->Init())) {
    LOG(ERROR) << "Init ao fail";
  } else if (common::Error::SUCCESS != (ret = vo_->Init(input_ctx_))) {
    LOG(ERROR) << "Init vo fail";
  } else {
    AdjustHZ(GET_CONFIG(default_tick_hz));
  }

  return ret;
}

common::Error PlayerContext::Run() {
  auto ret = common::Error::SUCCESS;

  runners.emplace_back(misc::Future::CreateFutureNode(demux_ctx_));
  runners.emplace_back(misc::Future::CreateFutureNode(ao_));
  runners.emplace_back(misc::Future::CreateFutureNode(vo_));
  BLOCKING_PUSH_TO_SLOT(ENTRY_SLOT,
                        std::make_shared<player::PlayEntry>
                   (player::entry_type::file, video_sample, 0));
  do {} while (Loop());
  return ret;
}

common::Error PlayerContext::Stop() {
  auto ret = common::Error::SUCCESS;

  demux_ctx_->Stop();
  ao_->Stop();
  vo_->Stop();
  for (auto &runner : runners) {
    runner->Join();
  }
  return ret;
}

bool PlayerContext::LoopImpl() {
  auto ret = common::Error::SUCCESS;

  /// some vo function must be called in main loop
  vo_->LoopInMainThread();

  if (common::Error::SUCCESS != (ret = input_ctx_->HandleEvent(event_handler_))) {
    if (ret != common::Error::EXIT) {
      LOG(WARNING) << "exit with err code " << static_cast<int64_t>(ret);
    }
  }
  return common::Error::SUCCESS == ret;
}

}
