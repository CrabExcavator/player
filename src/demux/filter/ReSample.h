//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_RESAMPLE_H
#define PLAYER_RESAMPLE_H

#include "misc/typeptr.h"
#include "FrameFilterBase.h"

namespace demux::filter {

    /**
     * @brief resample audio that device not support
     */
    class ReSample: public FrameFilterBase {
    public:
        ReSample() = default;
        void init();
        misc::vector_sptr<frame_sptr> filter(const misc::vector_sptr<frame_sptr>& in) override;
        misc::vector_sptr<frame_sptr> flush(const misc::vector_sptr<frame_sptr>& in) override;
        void close() override;
    };

}

#endif //PLAYER_RESAMPLE_H
