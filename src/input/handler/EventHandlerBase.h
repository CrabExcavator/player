//
// Created by CrabExcavator on 2020/11/3.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_EVENTHANDLERBASE_H
#define PLAYER_EVENTHANDLERBASE_H

#include "common/error.h"
#include "misc/ChainNode.h"
#include "misc/typeptr.h"

namespace input::handler {

    /**
     * @brief abstract class to handle event
     */
    class EventHandlerBase: public misc::ChainNode<input_ctx_sptr> {
    public:
        /**
         * @brief handle event
         * @param [in] in input context
         * @return input context
         */
        common::error filter(const misc::vector_sptr<input_ctx_sptr>& in, misc::vector_sptr<input_ctx_sptr>& out) override = 0;

        /**
         * @deprecated
         * @brief
         * @param in
         * @return
         */
        common::error flush(const misc::vector_sptr<input_ctx_sptr>& in, misc::vector_sptr<input_ctx_sptr>& out) override;

        /**
         * @deprecated
         */
        common::error close() override;

    protected:
        static core::player_ctx_sptr getPlayContext(const input_ctx_sptr& input_ctx);
    };

    using event_handler_chain_sptr = misc::chain_sptr<input_ctx_sptr>;

}

#endif //PLAYER_EVENTHANDLERBASE_H
