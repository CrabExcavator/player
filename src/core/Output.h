//
// Created by CrabExcavator on 2020/11/4.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_OUTPUT_H
#define PLAYER_OUTPUT_H

#include <cstdint>

#include "core/SyncContext.h"
#include "misc/typeptr.h"

namespace core {

    class Output {
    public:
        Output() = default;

    public:
        uint64_t version = 0;
        core::sync_sptr sync_;
    };

}

#endif //PLAYER_OUTPUT_H
