//
// Created by weihan on 2020/10/14.
//

#ifndef PLAYER_CHAIN_H
#define PLAYER_CHAIN_H

#include <deque>
#include <memory>

#include "ChainNode.h"

namespace misc {

    template <typename T>
    class Chain: public std::enable_shared_from_this<Chain<T>> {
    public:
        Chain(): _queue() {}

        chain_sptr<T> addLast(chain_node_sptr<T> node) {
            this->_queue.emplace_back(std::move(node));
            return this->shared_from_this();
        }

        chain_sptr<T> addFirst(chain_node_sptr<T> node) {
            this->_queue.emplace_front(std::move(node));
            return this->shared_from_this();
        }

        vector_sptr<T> filter(vector_sptr<T> in) {
            auto param = in;
            for (auto& node : this->_queue) {
                param = node->filter(param);
            }
            return param;
        }

        vector_sptr<T> flush() {
            vector_sptr<T> param{};
            for (auto& node : this->_queue) {
                param = node->flush(param);
            }
            return param;
        }

        void close() {
            for (auto& node : this->_queue) {
                node->close();
            }
        }

    private:
        std::deque<chain_node_sptr<T>> _queue;
    };

}

#endif //PLAYER_CHAIN_H
