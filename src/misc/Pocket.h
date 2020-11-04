//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_POCKET_H
#define PLAYER_POCKET_H

#include <string>
#include <sstream>

namespace misc {

    /**
     * @brief Pocket is a K-V data structure,
     * where key is std::string, as value is typename T
     * @todo how to use pocket
     * @tparam T type of key
     */
    template <typename T>
    class Pocket {
    public:
        /**
         * @brief deserialize std::string to value of typename T
         * @param [in] value
         * @return the casted value
         */
        static T cast(const std::string& value) {
            std::stringstream ss(value);
            T ret;
            ss >> ret;
            return ret;
        }

        /**
         * @brief setNumOfStream pocket with key and value
         * @param [in] key
         * @param [in] value
         */
        Pocket(std::string key, T value): _key(std::move(key)), _value(std::move(value)) {}

        /**
         * @brief delete
         */
        Pocket() = delete;

        /**
         * @brief default
         * @param rhs
         */
        Pocket(const Pocket<T>& rhs): _key(rhs._key), _value(rhs._value) {

        }

        /**
         * @brief default
         * @param rhs
         */
        Pocket(Pocket<T>&& rhs) noexcept: _key(std::move(rhs._key)), _value(std::move(rhs._value)) {

        }

        /**
         * @brief default
         * @param rhs
         * @return
         */
        Pocket<T>& operator = (const Pocket<T>& rhs) {
            if (this == &rhs) {
                return *this;
            }
            this->_key = rhs._key;
            this->_value = rhs._value;
            return *this;
        }

        /**
         * @brief default
         * @param rhs
         * @return
         */
        Pocket<T>& operator = (Pocket<T>&& rhs) noexcept {
            this->_key = std::move(rhs._key);
            this->_value = std::move(rhs._value);
            return *this;
        }

        /**
         * @brief default
         */
        ~Pocket() = default;

        /**
         * @brief get key
         * @return key
         */
        [[nodiscard]] const std::string& getKey() const {
            return this->_key;
        }

        /**
         * @brief get value
         * @return value
         */
        const T& getValue() const {
            return this->_value;
        }

    private:
        /**
         * @brief key
         */
        std::string _key;

        /**
         * @brief value
         */
        T _value;
    };

}

#endif //PLAYER_POCKET_H
