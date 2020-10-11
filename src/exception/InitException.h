//
// Created by weihan on 2020/10/11.
//

#ifndef PLAYER_INITEXCEPTION_H
#define PLAYER_INITEXCEPTION_H

#include <exception>

namespace exception {

    struct InitException : public std::exception {};

    struct DemuxerInitException : public InitException {};

    struct StreamInitException : public InitException {};

    struct SDLInitException : public InitException {};

}
#endif //PLAYER_INITEXCEPTION_H
