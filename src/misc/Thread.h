//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_THREAD_H
#define PLAYER_THREAD_H

#include <string>
#include <memory>
#include <thread>
#include <glog/logging.h>

namespace misc {

    class Thread;
    using thread_uptr = std::unique_ptr<Thread>;

    class Thread {
#define MISC_THREAD_TT "Thread " << _thread_name << " "
    public:
        Thread() = default;
        Thread(const Thread& rhs) = delete;
        Thread(Thread&& rhs) noexcept {
            this->_thread_name = std::move(rhs._thread_name);
            this->_thread = std::move(rhs._thread);
        }
        Thread& operator = (const Thread& rhs) = delete;
        Thread& operator = (Thread&& rhs) = delete;
        explicit Thread(std::string thread_name): _thread_name(std::move(thread_name)) {
            //LOG(INFO) << MISC_THREAD_TT << "create";
        }
        ~Thread() {
            //LOG(INFO) << MISC_THREAD_TT << "deCreate";
        };

        template<typename Fp, typename ...Args>
        void run(Fp&& callable, Args&&... args) {
            //LOG(INFO) << MISC_THREAD_TT << "start";
            this->_thread = std::move(std::thread(std::forward<Fp, Args...>(callable, args...)));
        }

        void join() {
            this->_thread.join();
            //LOG(INFO) << MISC_THREAD_TT << "join";
        }

    private:
        std::string _thread_name;
        std::thread _thread;
    };

}

#endif //PLAYER_THREAD_H
