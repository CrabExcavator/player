//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_PLAYENTRY_H
#define PLAYER_PLAYENTRY_H

#include "misc/typeptr.h"

#include <string>
#include <memory>

namespace player {

/**
 * @brief supported entry type
 */
enum class entry_type {
  file,
  numOfEntryType
};

/**
 * @brief play entry
 */
class PlayEntry {
 public:
  /**
   * @brief delete
   */
  PlayEntry() = delete;

  /**
   * @brief setNumOfStream play entry
   * @param [in] type entry type
   * @param [in] uri resource path
   * @param [in] last_pts no use yet
   */
  PlayEntry(entry_type type, std::string uri, int64_t last_pts);

  /**
   * @brief default
   * @param rhs
   */
  PlayEntry(const PlayEntry &rhs) = default;

  /**
   * @brief default
   * @param rhs
   */
  PlayEntry(PlayEntry &&rhs) = default;

  /**
   * @brief default
   * @param rhs
   * @return
   */
  PlayEntry &operator=(const PlayEntry &rhs) = default;

  /**
   * @brief default
   * @param rhs
   * @return
   */
  PlayEntry &operator=(PlayEntry &&rhs) = default;

  /**
   * @brief default
   */
  ~PlayEntry() = default;

  /**
   * @brief cmp operator
   * @param rhs
   * @return
   */
  bool operator<(const PlayEntry &rhs) const {
    return (this->type < rhs.type) || (this->type == rhs.type && this->uri < rhs.uri);
  }

 public:
  /**
   * @brief entry type
   */
  entry_type type;

  /**
   * @brief resource path
   */
  std::string uri;

  /**
   * @brief pts last play
   */
  int64_t last_pts;
};

}

#endif //PLAYER_PLAYENTRY_H
