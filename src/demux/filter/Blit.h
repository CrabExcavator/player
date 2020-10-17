//
// Created by weihan on 2020/10/15.
//

#ifndef PLAYER_BLIT_H
#define PLAYER_BLIT_H

#include "FrameFilterBase.h"
#include "video/image_format.h"

namespace demux::filter {

    class Blit: public FrameFilterBase {
    public:
        misc::vector_sptr<frame_sptr> filter(const misc::vector_sptr<frame_sptr>& in) override;
        misc::vector_sptr<frame_sptr> flush(const misc::vector_sptr<frame_sptr>& in) override;
        void close() override;
    };

}

#endif //PLAYER_BLIT_H
