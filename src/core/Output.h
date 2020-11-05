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

    /**
     * @brief enum output
     */
    enum class output_port {
        null = 0, /**< enum value is 0 */
        video = 1, /**< enum value is 1 */
        audio = 2, /**< enum value is 2 */
    };

    /**
     * @brief output base
     */
    class Output {
    public:
        Output() = default;

    protected:
        uint64_t _version = 0;
        core::sync_ctx_sptr _sync_ctx = nullptr;
        demux::stream::stream_sptr _stream = nullptr;
    };

}

#endif //PLAYER_OUTPUT_H
