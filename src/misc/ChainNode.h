//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_CHAINNODE_H
#define PLAYER_CHAINNODE_H

#include <vector>

#include "typeptr.h"

namespace misc {

    /**
     * @brief chain node
     * @tparam T type to handle
     */
    template <typename T>
    class ChainNode {
    public:
        /**
         * @brief filter
         * @param [in] in list of typename T
         * @return list of typename T
         */
        virtual vector_sptr<T> filter(const vector_sptr<T>& in) = 0;

        /**
         * @brief flush
         * @param [in] in list of typename T
         * @return list of typename T
         */
        virtual vector_sptr<T> flush(const vector_sptr<T>& in) = 0;

        /**
         * @brief close
         */
        virtual void close() = 0;
    };

}

#endif //PLAYER_CHAINNODE_H
