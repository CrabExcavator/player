//
// Created by weihan on 2020/10/6.
//

#include <gtest/gtest.h>

#include "misc/Thread.h"
#include "misc/Lock.h"
#include "misc/LockGuard.h"
#include "misc/util.h"

TEST(SAMPLE, THREAD) {
    misc::Thread t1(std::string("hehe"));
    misc::Thread t2(std::move(t1));
    t2.run([](){
        LOG(INFO) << "Hello Thread";
    });
    t2.join();
}

TEST(SAMPLE, LOCK) {
    auto thread_num = 10;
    auto loop_num = 10000;
    misc::Lock lock("sample");
    int a = 0;
    std::vector<misc::thread_uptr> threads;
    threads.reserve(thread_num);
    for (int i = 0 ; i < thread_num ; i++) {
        threads.emplace_back(std::make_unique<misc::Thread>(misc::to_string("sample", i)));
    }
    for (auto& thread : threads) {
        thread->run([&](){
            for (int i = 0 ; i < loop_num ; i++) {
                misc::LockGuard lock_guard(lock);
                a++;
            }
        });
    }
    for (auto& thread : threads) {
        thread->join();
    }
    ASSERT_EQ(a, thread_num*loop_num);
}
