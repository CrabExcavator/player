//
// Created by weihan on 2020/10/31.
//

#ifndef PLAYER_SYNC_H
#define PLAYER_SYNC_H

#include <mutex>

namespace core {

    class Sync {
    public:
        Sync();
        void init(int size);
        void wait();
        void close();

    private:
        bool _close;
        int _size;
        int _cnt;
        std::mutex _mutex;
        std::condition_variable _cond;
    };

}

#endif //PLAYER_SYNC_H
