//
// Created by weihan on 2020/10/8.
//

#ifndef PLAYER_POCKET_H
#define PLAYER_POCKET_H

#include <string>
#include <sstream>

namespace misc {

    template <typename T>
    class Pocket {
    public:
        static T cast(const std::string& value) {
            std::stringstream ss(value);
            T ret;
            ss >> ret;
            return ret;
        }

        Pocket(std::string key, T value): _key(std::move(key)), _value(std::move(value)) {}

        Pocket() = delete;

        Pocket(const Pocket<T>& rhs): _key(rhs._key), _value(rhs._value) {

        }

        Pocket(Pocket<T>&& rhs) noexcept: _key(std::move(rhs._key)), _value(std::move(rhs._value)) {

        }

        Pocket<T>& operator = (const Pocket<T>& rhs) {
            if (this == &rhs) {
                return *this;
            }
            this->_key = rhs._key;
            this->_value = rhs._value;
            return *this;
        }

        Pocket<T>& operator = (Pocket<T>&& rhs) noexcept {
            this->_key = std::move(rhs._key);
            this->_value = std::move(rhs._value);
            return *this;
        }

        ~Pocket() = default;

        [[nodiscard]] const std::string& getKey() const {
            return this->_key;
        }

        const T& getValue() const {
            return this->_value;
        }

    private:
        std::string _key;
        T _value;
    };

}

#endif //PLAYER_POCKET_H
