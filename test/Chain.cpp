//
// Created by weihan on 2020/10/14.
//

#include <gtest/gtest.h>

#include "misc/Chain.h"

class ChainNodeSample : public misc::ChainNode<ChainNodeSample> {
public:
    misc::vector_sptr<ChainNodeSample> filter(const misc::vector_sptr<ChainNodeSample>& in) override {return nullptr;}
    misc::vector_sptr<ChainNodeSample> flush(const misc::vector_sptr<ChainNodeSample>& in) override {return nullptr;}
    void close() override {}
};

TEST(CHAIN, CHAIN) {
    auto chain = std::make_shared<misc::Chain<ChainNodeSample>>();
    chain->addLast(std::make_shared<ChainNodeSample>())->addLast(std::make_shared<ChainNodeSample>())
    ->addFirst(std::make_shared<ChainNodeSample>());
}
