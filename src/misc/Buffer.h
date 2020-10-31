//
// Created by weihan on 2020/10/24.
//

#ifndef PLAYER_BUFFER_H
#define PLAYER_BUFFER_H

#include <algorithm>
#include <glog/logging.h>
#include <mutex>
#include <condition_variable>
#include <chrono>

#include "misc/util.h"
#include "typeptr.h"

namespace misc {

    // todo stream buffer
    template<typename T, size_t Size>
    class Buffer {
    public:
        Buffer() = default;
        Buffer(const Buffer& rhs) {
            this->_array = rhs._array;
        }
        Buffer(Buffer&& rhs) noexcept {
            this->_array = std::move(rhs._array);
        }
        Buffer& operator = (const Buffer& rhs) {
            this->_array = rhs._array;
        }
        Buffer& operator = (Buffer&& rhs) noexcept {
            this->_array = std::move(rhs._array);
        }

        void put(const T* src, int beginOfEle, int numOfEle) {
            std::unique_lock<std::mutex> lock(this->_mutex);
            while(!this->_close) {
                auto flag = this->_cond.wait_for(lock, std::chrono::milliseconds(500), [&](){return this->_put_cond(numOfEle);});
                if (this->_close) return;
                if (flag) {
                    break;
                } else {
                    LOG(INFO) << "put continue";
                    continue;
                }
            }
            int nxt_tail = this->_tail + numOfEle;
            for (int i = 0 ; i < numOfEle ; i++) {
                this->_array[(this->_tail + i) % Size] = src[beginOfEle + i];
            }
            this->_tail = nxt_tail % Size;
            this->_buffered_ele += numOfEle;
            lock.unlock();
            this->_cond.notify_one();
        }

        template<size_t oSize>
        void put(const std::array<T, oSize>& src, int beginOfEle, int numOfEle) {
            std::unique_lock<std::mutex> lock(this->_mutex);
            while(!this->_close) {
                auto flag = this->_cond.wait_for(lock, std::chrono::milliseconds(500), [&](){return this->_put_cond(numOfEle);});
                if (this->_close) return;
                if (flag) {
                    break;
                } else {
                    continue;
                }
            }
            int nxt_tail = this->_tail + numOfEle;
            for (int i = 0 ; i < numOfEle ; i++) {
                this->_array[(this->_tail + i) % Size] = src[(beginOfEle + i) % oSize];
            }
            this->_tail = nxt_tail % Size;
            this->_buffered_ele += numOfEle;
            lock.unlock();
            this->_cond.notify_one();
        }

        void get(T* dst, int beginOfEle, int numOfEle) {
            std::unique_lock<std::mutex> lock(this->_mutex);
            while(!this->_close) {
                auto flag = this->_cond.wait_for(lock, std::chrono::milliseconds(500), [&](){return this->_get_cond(numOfEle);});
                if (this->_close) return;
                if (flag) {
                    break;
                } else {
                    continue;
                }
            }
            int nxt_head = this->_head + numOfEle;
            for (int i = 0 ; i < numOfEle ; i++) {
                dst[beginOfEle + i] = this->_array[(this->_head + i) % Size];
            }
            this->_head = nxt_head % Size;
            this->_buffered_ele -= numOfEle;
            lock.unlock();
            this->_cond.notify_one();
        }

        template<size_t oSize>
        void get(std::array<T, oSize>& dst, int beginOfEle, int numOfEle) {
            std::unique_lock<std::mutex> lock(this->_mutex);
            while(!this->_close) {
                auto flag = this->_cond.wait_for(lock, std::chrono::milliseconds(500), [&](){return this->_get_cond(numOfEle);});
                if (this->_close) return;
                if (flag) {
                    break;
                } else {
                    continue;
                }
            }
            int nxt_head = this->_head + numOfEle;
            for (int i = 0 ; i < numOfEle ; i++) {
                dst[(beginOfEle + i) % oSize] = this->_array[(this->_head + i) % Size];
            }
            this->_head = nxt_head % Size;
            this->_buffered_ele -= numOfEle;
            lock.unlock();
            this->_cond.notify_one();
        }

        void close() {
            this->_close = true;
        }

        int size() {
            return this->_buffered_ele;
        }

    private:
        inline bool _get_cond(int numOfEle) {
            return this->_buffered_ele >= numOfEle;
        }

        inline bool _put_cond(int numOfEle) {
            return numOfEle + this->_buffered_ele <= Size;
        }

    private:
        std::atomic<bool> _close = false;
        std::mutex _mutex;
        std::condition_variable _cond;
        int _head = 0;
        int _tail = 0;
        int _buffered_ele = 0;
        std::array<T, Size> _array;
    };

}

#endif //PLAYER_BUFFER_H
