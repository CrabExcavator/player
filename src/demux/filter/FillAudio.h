//
// Created by CrabExcavator on 2020/11/5.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_FILLAUDIO_H
#define PLAYER_FILLAUDIO_H

#include "FrameFilterBase.h"

namespace demux::filter {

    class FillAudio : public FrameFilterBase {
    public:
        FillAudio() = default;

        common::error filter(const misc::vector_sptr<frame_sptr>& in, misc::vector_sptr<frame_sptr>& out) override;

        common::error flush(const misc::vector_sptr<frame_sptr>& in, misc::vector_sptr<frame_sptr>& out) override;

        common::error close() override;
    };

}

#endif //PLAYER_FILLAUDIO_H
