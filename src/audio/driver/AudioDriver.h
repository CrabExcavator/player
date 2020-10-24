//
// Created by weihan on 2020/10/20.
//

#ifndef PLAYER_AUDIODRIVER_H
#define PLAYER_AUDIODRIVER_H

#include <memory>

#include "misc/typeptr.h"

namespace audio::driver {

    class AudioDriver {
    public:
        AudioDriver() = default;

        virtual ~AudioDriver() = default;

        virtual void init(ao_sptr ao) = 0;

        virtual void play(ao_sptr ao) = 0;

        virtual void stop(ao_sptr ao) = 0;

        virtual void reConfig(ao_sptr ao) = 0;

        virtual misc::vector_sptr<std::string> getDevices(ao_sptr ao) = 0;
    };

}

#endif //PLAYER_AUDIODRIVER_H
