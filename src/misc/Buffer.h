//
// Created by weihan on 2020/10/24.
//

#ifndef PLAYER_BUFFER_H
#define PLAYER_BUFFER_H

#include <algorithm>
#include <glog/logging.h>

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
            this->_cond.wait(lock, [&](){
                return numOfEle + this->_buffered_ele <= Size;
            });
            int nxt_tail = this->_tail + numOfEle;
//            for(int i = this->_tail ; i < nxt_tail ; i++) {
//                this->_array[i % Size] = src[beginOfEle + i];
//            }
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
            this->_cond.wait(lock, [&](){
                return numOfEle + this->_buffered_ele <= Size;
            });
            int nxt_tail = this->_tail + numOfEle;
//            for(int i = this->_tail ; i < nxt_tail ; i++) {
//                this->_array[i % Size] = src[(beginOfEle + i) % oSize];
//            }
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
            this->_cond.wait(lock, [&](){
                return this->_buffered_ele >= numOfEle;
            });
            int nxt_head = this->_head + numOfEle;
//            for (int i = this->_head ; i < nxt_head ; i++) {
//                dst[beginOfEle + i] = this->_array[i % Size];
//            }
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
            this->_cond.wait(lock, [&](){
                return this->_buffered_ele >= numOfEle;
            });
            int nxt_head = this->_head + numOfEle;
//            for (int i = this->_head ; i < nxt_head ; i++) {
//                dst[(beginOfEle + i) % oSize] = this->_array[i % Size];
//            }
            for (int i = 0 ; i < numOfEle ; i++) {
                dst[(beginOfEle + i) % oSize] = this->_array[(this->_head + i) % Size];
            }
            this->_head = nxt_head % Size;
            this->_buffered_ele -= numOfEle;
            lock.unlock();
            this->_cond.notify_one();
        }

        int size() {
            return this->_buffered_ele;
        }
    private:
        std::mutex _mutex;
        std::condition_variable _cond;
        int _head = 0;
        int _tail = 0;
        int _buffered_ele = 0;
        std::array<T, Size> _array;
    };

}

#endif //PLAYER_BUFFER_H
