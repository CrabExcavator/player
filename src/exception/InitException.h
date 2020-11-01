//
// Created by weihan on 2020/10/11.
//

#ifndef PLAYER_INITEXCEPTION_H
#define PLAYER_INITEXCEPTION_H

#include <exception>

// todo C++ exception is fxxking stupid, we should use traditional return err code && Init shouldn't throw exception
namespace exception {

    struct InitException : public std::exception {};

    struct DemuxerInitException : public InitException {};

    struct StreamInitException : public InitException {};

    struct SDLInitException : public InitException {};

}
#endif //PLAYER_INITEXCEPTION_H
