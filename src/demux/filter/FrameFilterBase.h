//
// Created by weihan on 2020/10/15.
//

#ifndef PLAYER_FRAMEFILTERBASE_H
#define PLAYER_FRAMEFILTERBASE_H

#include "misc/ChainNode.h"
#include "misc/Chain.h"
#include "misc/typeptr.h"
#include "demux/Frame.h"

namespace demux::filter {

    class FrameFilterBase: public misc::ChainNode<frame_sptr> {
    public:
        misc::vector_sptr<frame_sptr> filter(const misc::vector_sptr<frame_sptr>& in) override = 0;
        misc::vector_sptr<frame_sptr> flush(const misc::vector_sptr<frame_sptr>& in) override = 0;
        void close() override = 0;
    };

    using frame_filter_chain_sptr = misc::chain_sptr<frame_sptr>;

}

#endif //PLAYER_FRAMEFILTERBASE_H
