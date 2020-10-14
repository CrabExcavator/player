//
// Created by weihan on 2020/10/14.
//

#ifndef PLAYER_CHAINNODE_H
#define PLAYER_CHAINNODE_H

#include <vector>

#include "typeptr.h"

namespace misc {

    template <typename T>
    class ChainNode {
    public:
        virtual vector_sptr<T> filter(const vector_sptr<T>& in) = 0;
        virtual vector_sptr<T> flush(const vector_sptr<T>& in) = 0;
        virtual void close() = 0;
    };

}

#endif //PLAYER_CHAINNODE_H
