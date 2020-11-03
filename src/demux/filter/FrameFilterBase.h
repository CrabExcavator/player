//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_FRAMEFILTERBASE_H
#define PLAYER_FRAMEFILTERBASE_H

#include "misc/ChainNode.h"
#include "misc/typeptr.h"
#include "demux/Frame.h"

namespace demux::filter {

    /**
     * @brief abstract class to filter frame
     */
    class FrameFilterBase: public misc::ChainNode<frame_sptr> {
    public:
        /**
         * @brief filter frame
         * @param [in] in
         * @param [out] out
         * @return error code
         */
        common::error filter(const misc::vector_sptr<frame_sptr>& in, misc::vector_sptr<frame_sptr>& out) override = 0;

        /**
         * @brief flush frame
         * @param [in] in
         * @param [out] out
         * @return error code
         */
        common::error flush(const misc::vector_sptr<frame_sptr>& in, misc::vector_sptr<frame_sptr>& out) override = 0;

        /**
         * @brief close frame filter
         * @return error code
         */
        common::error close() override = 0;
    };

    using frame_filter_chain_sptr = misc::chain_sptr<frame_sptr>;

}

#endif //PLAYER_FRAMEFILTERBASE_H
