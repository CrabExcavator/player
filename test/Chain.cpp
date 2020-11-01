//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <string>
#include <gtest/gtest.h>
#include <glog/logging.h>

#include "misc/Chain.h"

class Sample {};

class ChainNodeSample : public misc::ChainNode<Sample> {
public:
    void init(std::string name) {
        this->_name = std::move(name);
    }
    misc::vector_sptr<Sample> filter(const misc::vector_sptr<Sample>& in) override {
        LOG(INFO) << "filter";
        return nullptr;
    }
    misc::vector_sptr<Sample> flush(const misc::vector_sptr<Sample>& in) override {
        LOG(INFO) << "flush";
        return nullptr;
    }
    void close() override {}

private:
    std::string _name;
};

class ChainNodeSampleDrived : public ChainNodeSample {
    misc::vector_sptr<Sample> filter(const misc::vector_sptr<Sample>& in) override {
        LOG(INFO) << "drived filter";
        return nullptr;
    }
};

TEST(CHAIN, CHAIN) {
    auto chain = std::make_shared<misc::Chain<Sample>>();
    chain->addLast(std::make_shared<ChainNodeSample>())->addLast(std::make_shared<ChainNodeSample>())
    ->addFirst(std::make_shared<ChainNodeSample>())->addFirst(std::make_shared<ChainNodeSampleDrived>());
    chain->filter(nullptr);
}
