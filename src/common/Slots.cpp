//
// Created by CrabExcavator on 2020/11/20.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "Slots.h"

namespace common {

#define DEFINE_SLOT(slot_num, type) Slot<type> slot_##slot_num

DEFINE_SLOT(ENTRY_SLOT, player::play_entry_sptr);

DEFINE_SLOT(AUDIO_OUTPUT_CTL_SLOT, common::Signal);
DEFINE_SLOT(AUDIO_OUTPUT_STREAM_SLOT, demux::stream::stream_sptr);

}
