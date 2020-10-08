//
// Created by weihan on 2020/10/8.
//

#include <gtest/gtest.h>

#include "misc/Pocket.h"
#include "common/Config.h"
#include "common/DefaultConfig.h"

TEST(POCKET, CREATE) {
    std::string key = "test";
    int value = 1;
    auto pocket = misc::Pocket<int>(key, value);
    auto pocket1 = std::move(pocket);
    const auto& pocket2(pocket1);
    ASSERT_EQ(key, pocket1.getKey());
    ASSERT_EQ(value, pocket1.getValue());
    ASSERT_EQ(key, pocket2.getKey());
    ASSERT_EQ(value, pocket2.getValue());
}

TEST(POCKET, DEFAULT) {
    ASSERT_EQ(GET_CONFIG(window_height), 480);
    ASSERT_EQ(GET_CONFIG(window_width), 640);
}