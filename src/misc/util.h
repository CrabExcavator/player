//
// Created by weihan on 2020/10/8.
//

#ifndef PLAYER_MISC_H
#define PLAYER_MISC_H

#include <string>
#include <sstream>

namespace misc {

    template <typename T>
    inline std::string to_string(T&& tail) {
        std::stringstream ss;
        std::string s;
        ss << tail;
        ss >> s;
        return s;
    }

    template <typename T, typename... Args>
    inline std::string to_string(T&& head, Args&&... args) {
        std::stringstream ss;
        std::string s;
        ss << head;
        ss >> s;
        return s + to_string(std::forward<Args>(args)...);
    }

}

#endif //PLAYER_MISC_H
