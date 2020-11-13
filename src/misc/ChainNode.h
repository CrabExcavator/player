//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_CHAINNODE_H
#define PLAYER_CHAINNODE_H

#include <vector>

#include "common/Error.h"
#include "typeptr.h"

namespace misc {

/**
 * @brief chain node
 * @tparam T type to handle
 */
template<typename T>
class ChainNode {
 public:
  /**
   * @brief filter
   * @param [in] in list of typename T
   * @param [out] out list of typename T
   * @return error code
   */
  virtual common::Error filter(const vector_sptr<T> &in, vector_sptr<T> &out) = 0;

  /**
   * @brief flush
   * @param [in] in list of typename T
   * @param [out] out list of typename T
   * @return error code
   */
  virtual common::Error flush(const vector_sptr<T> &in, vector_sptr<T> &out) = 0;

  /**
   * @brief close
   * @return error code
   */
  virtual common::Error close() = 0;
};

}

#endif //PLAYER_CHAINNODE_H
