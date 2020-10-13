//
// Created by weihan on 2020/10/8.
//

#ifndef PLAYER_MISC_H
#define PLAYER_MISC_H

#include <string>
#include <sstream>

namespace misc {

    template <typename T>
    inline std::string to_string(T&& head) {
        std::stringstream ss;
        ss << head;
        return ss.str();
    }

    template <typename T, typename... Args>
    inline std::string to_string(T&& head, Args&&... args) {
        return to_string(std::forward<T>(head)) + to_string(std::forward<Args>(args)...);
    }

}

#endif //PLAYER_MISC_H
