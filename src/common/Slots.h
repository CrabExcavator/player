//
// Created by CrabExcavator on 2020/11/20.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_MISC_SLOTS_H_
#define PLAYER_SRC_MISC_SLOTS_H_

#include <array>
#include <map>
#include <folly/MPMCQueue.h>

#include "common/Error.h"
#include "common/Signal.h"
#include "misc/typeptr.h"

namespace common {

template <typename T>
class Slot {
 public:
  Slot() : slot_(1024) {}

  inline void BlockingPush(T ele) {
    slot_.blockingWrite(ele);
  }

  inline T BlockingGet() {
    T ele;
    slot_.blockingRead(ele);
    return ele;
  }

  inline bool Get(T &ele) {
    return slot_.read(ele);
  }

 private:
  folly::MPMCQueue<T> slot_;
};

#define DECLARE_SLOT(slot_num, type) extern Slot<type> slot_##slot_num

#define BLOCKING_PUSH_TO_SLOT(slot_num, ele) common::slot_##slot_num.BlockingPush(ele)

#define BLOCKING_GET_FROM_SLOT(slot_num) common::slot_##slot_num.BlockingGet()

#define GET_FROM_SLOT(slot_num, ele) common::slot_##slot_num.Get(ele)

#define ENTRY_SLOT 101
DECLARE_SLOT(ENTRY_SLOT, player::play_entry_sptr);

#define VIDEO_OUTPUT_CTL_SLOT 201
#define VIDEO_OUTPUT_STREAM_SLOT 302
DECLARE_SLOT(VIDEO_OUTPUT_CTL_SLOT, common::Signal);
DECLARE_SLOT(VIDEO_OUTPUT_STREAM_SLOT, demux::stream::stream_sptr);

#define AUDIO_OUTPUT_CTL_SLOT 301
#define AUDIO_OUTPUT_STREAM_SLOT 302
DECLARE_SLOT(AUDIO_OUTPUT_CTL_SLOT, common::Signal);
DECLARE_SLOT(AUDIO_OUTPUT_STREAM_SLOT, demux::stream::stream_sptr);

}

#endif //PLAYER_SRC_MISC_SLOTS_H_
