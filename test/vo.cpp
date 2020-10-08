//
// Created by weihan on 2020/10/7.
//

#include <gtest/gtest.h>
#include <glog/logging.h>

#include "input/InputContext.h"
#include "video/VideoOutput.h"

TEST(vo, create) {
    auto inputCtx = std::make_shared<input::InputContext>();
    auto vo = std::make_shared<video::VideoOutput>(inputCtx);
}