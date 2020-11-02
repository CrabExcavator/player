//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
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

    /**
     * @brief a templated data structure to buffer data
     *
     * @tparam T element of Buffer
     * @tparam Size size of Buffer
     */
    template<typename T, size_t Size>
    class Buffer {
    public:
        /**
         * @brief default
         */
        Buffer() = default;

        /**
         * @brief copy array
         * @param rhs
         */
        Buffer(const Buffer& rhs) {
            this->_array = rhs._array;
        }

        /**
         * @brief move array
         * @param rhs
         */
        Buffer(Buffer&& rhs) noexcept {
            this->_array = std::move(rhs._array);
        }

        /**
         * @brief copy array
         * @param rhs
         * @return
         */
        Buffer& operator = (const Buffer& rhs) {
            this->_array = rhs._array;
        }

        /**
         * @brief move array
         * @param rhs
         * @return
         */
        Buffer& operator = (Buffer&& rhs) noexcept {
            this->_array = std::move(rhs._array);
        }

        /**
         * @brief put ele in array with copy fashion
         * @param [in] src head of src
         * @param [in] beginOfEle begin of ele to copy
         * @param [in] numOfEle number of ele to copy
         */
        void put(const T* src, int beginOfEle, int numOfEle) {
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
                this->_array[(this->_tail + i) % Size] = src[beginOfEle + i];
            }
            this->_tail = nxt_tail % Size;
            this->_buffered_ele += numOfEle;
            lock.unlock();
            this->_cond.notify_one();
        }

        /**
         * @brief put ele in array with copy fashion
         * @tparam oSize the size of src array
         * @param [in] src src array to copy
         * @param [in] beginOfEle begin of ele to copy
         * @param [in] numOfEle number of ele to copy
         */
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

        /**
         * @brief get ele from array to dst
         * @param [out] dst pointer to dst
         * @param [in] beginOfEle begin of ele to put
         * @param [in] numOfEle number of ele to put
         */
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

        /**
         * @brief get ele from array to dst
         * @tparam oSize size of dst array
         * @param [out] dst pointer to dst
         * @param [in] beginOfEle begin of ele to put
         * @param [in] numOfEle number of ele to put
         */
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

        /**
         * @brief clear all ele
         */
        void clear() {
            this->_mutex.lock();
            DEFER([&](){this->_mutex.unlock();});
            this->_close = false;
            this->_head = 0;
            this->_tail = 0;
            this->_buffered_ele = 0;
        }

        /**
         * @brief close buffer and release all choke thread
         */
        void close() {
            this->_close = true;
        }

        /**
         * @brief size of buffer
         * @return size of buffer
         */
        int size() {
            return this->_buffered_ele;
        }

    private:
        /**
         * @brief internal cond
         * @param numOfEle
         * @return
         */
        inline bool _get_cond(int numOfEle) {
            return this->_buffered_ele >= numOfEle;
        }

        /**
         * @brief internal cond
         * @param numOfEle
         * @return
         */
        inline bool _put_cond(int numOfEle) {
            return numOfEle + this->_buffered_ele <= Size;
        }

    private:
        /**
         * @brief is buffer close
         */
        std::atomic<bool> _close = false;

        /**
         * @brief mutex
         */
        std::mutex _mutex;

        /**
         * @brief cond
         */
        std::condition_variable _cond;

        /**
         * @brief head of ele
         */
        int _head = 0;

        /**
         * @brief tail of ele
         */
        int _tail = 0;

        /**
         * @brief size of buffered ele
         */
        int _buffered_ele = 0;

        /**
         * @brief ele array
         */
        std::array<T, Size> _array;
    };

}

#endif //PLAYER_BUFFER_H
