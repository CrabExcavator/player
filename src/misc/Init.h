//
// Created by weihan on 2020/10/10.
//

#ifndef PLAYER_INIT_H
#define PLAYER_INIT_H

#include <glog/logging.h>
#include <gflags/gflags.h>

namespace misc {

    class Init {
    public:
        Init() = delete;


        Init(int argc, char* argv[]) {
            google::InitGoogleLogging(argv[0]);
            FLAGS_logtostderr = true;
            gflags::ParseCommandLineFlags(&argc, &argv, true);
        }

        ~Init() {
            google::ShutdownGoogleLogging();
        }
    };

}

#endif //PLAYER_INIT_H
