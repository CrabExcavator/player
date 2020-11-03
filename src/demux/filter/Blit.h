//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_BLIT_H
#define PLAYER_BLIT_H

#include "FrameFilterBase.h"
#include "video/image_format.h"

namespace demux::filter {

    /**
     * @brief transfer data in raw frame to data that can be used directly
     */
    class Blit: public FrameFilterBase {
    public:
        common::error filter(const misc::vector_sptr<frame_sptr>& in, misc::vector_sptr<frame_sptr>& out) override;
        common::error flush(const misc::vector_sptr<frame_sptr>& in, misc::vector_sptr<frame_sptr>& out) override;
        common::error close() override;
    };

}

#endif //PLAYER_BLIT_H
