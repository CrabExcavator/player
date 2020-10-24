//
// Created by weihan on 2020/10/20.
//

#include <memory>

#include "DriverFactory.h"
#include "DriverPortAudio.h"

namespace audio::driver {

    audio_driver_uptr DriverFactory::create(const std::string& driver_name) {
        return std::make_unique<DriverPortAudio>();
    }

}
