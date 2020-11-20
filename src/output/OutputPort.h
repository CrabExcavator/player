//
// Created by CrabExcavator on 2020/11/4.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_OUTPUT_H
#define PLAYER_OUTPUT_H

#include <cstdint>

#include "misc/typeptr.h"

namespace output {

/**
 * @brief enum output
 */
enum class OutputPort {
  EMPTY = 0, /**< enum value is 0 */
  VIDEO = 1, /**< enum value is 1 */
  AUDIO = 2, /**< enum value is 2 */
};

}

#endif //PLAYER_OUTPUT_H
