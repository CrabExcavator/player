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

namespace input {

    using event_set = std::bitset<static_cast<size_t>(event::numOfEvent)>;

    /**
     * @brief input context
     */
    class InputContext {
    public:
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
         * @return
         */
        bool hasEvent(event ev);

        /**
         * @brief clear event set
         */
        void clear();

    private:
        /**
         * @brief event set
         */
        event_set _slots;
    };
    
}

#endif //PLAYER_INPUTCONTEXT_H
