//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_INPUTCONTEXT_H
#define PLAYER_INPUTCONTEXT_H

#include <bitset>
#include <memory>
#include <glog/logging.h>

#include "event.h"
#include "common/error.h"
#include "misc/typeptr.h"
#include "handler/EventHandlerBase.h"

namespace input {

    using event_set = std::bitset<static_cast<size_t>(event::numOfEvent)>;

    /**
     * @brief input context
     */
    class InputContext : public std::enable_shared_from_this<InputContext> {
    public:
        friend handler::EventHandlerBase;
        /**
         * @brief default
         */
        InputContext();

        /**
         * @brief delete
         * @param rhs
         */
        InputContext(const InputContext& rhs) = delete;

        /**
         * @brief copy event set
         * @param rhs
         */
        InputContext(InputContext&& rhs) noexcept ;

        /**
         * @brief delete
         * @param rhs
         * @return
         */
        InputContext& operator = (const InputContext& rhs) = delete;

        /**
         * @brief copy event set
         * @param rhs
         * @return
         */
        InputContext& operator = (InputContext&& rhs) noexcept ;

        /**
         * @brief default
         */
        ~InputContext() = default;

        /**
         * @brief init
         */
         common::error init(const core::player_ctx_sptr& player);

        /**
         * @brief recv event
         * @param [in] ev event to set
         */
        void receive(event ev);

        /**
         * @brief poll all event
         * @return all event
         */
        event_set pollEvent();

        /**
         * @brief judge input context have specified event
         * @param [in] ev event to judge
         * @return if has event
         */
        bool hasEvent(event ev);

        /**
         * @brief clear event in event set
         * @param [in] ev event to clear
         */
        void clearEvent(event ev);

        /**
         * @brief clear event set
         */
        void clear();

        /**
         * @brief handle event
         */
         common::error handleEvent();

    private:
        /**
         * @brief event set
         */
        event_set _slots;

        /**
         * @brief chain to handle event
         */
        handler::event_handler_chain_sptr _handler_chain;

        /**
         * @brief weak ptr to player context
         */
        core::player_ctx_wptr _player_ctx;
    };
    
}

#endif //PLAYER_INPUTCONTEXT_H
