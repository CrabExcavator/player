//
// Created by weihan on 2020/10/20.
//

#ifndef PLAYER_DRIVERSDL_H
#define PLAYER_DRIVERSDL_H

#include "AudioDriver.h"

namespace audio::driver {

    class DriverSDL: public AudioDriver {
    public:
        ~DriverSDL() override;

        void init(ao_sptr ao) override;

        void playSound(ao_sptr ao) override;

        void reConfig(ao_sptr ao) override;
    };

}

#endif //PLAYER_DRIVERSDL_H
