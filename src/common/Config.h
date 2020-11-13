//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_CONFIG_H
#define PLAYER_CONFIG_H

#include <memory>
#include <unordered_map>
#include <mutex>

#include "misc/Pocket.h"
#include "common/DefaultConfig.h"
#include "misc/typeptr.h"

namespace common {

using dic_uptr = std::unique_ptr<std::unordered_map<std::string, std::string>>;

/**
 * @brief config
 */
class Config {
 public:
  /**
   * @brief get config singleton
   * @return config singleton
   */
  static config_sptr getInstance();

  /**
   * @brief load config from local filesystem
   * @param [in] path path of config file
   */
  static void loadConfig(const std::string &path);

  /**
   * @brief delete
   * @param rhs
   */
  Config(const Config &rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   */
  Config(Config &&rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  Config &operator=(const Config &rhs) = delete;

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  Config &operator=(Config &&rhs) = delete;

  /**
   * @brief default
   */
  ~Config() = default;

  /**
   * @brief key of config key-value pair
   * @tparam T type of value
   * @param [in] pocket wrapper of key
   * @return value
   */
  template<typename T>
  T get(const misc::Pocket<T> &pocket) {
    std::lock_guard _(this->_mutex);
    const auto &key = pocket.getKey();
    // todo add additional type support
//            if (this->_dic->contains(key)) {
//                return misc::Pocket<T>::cast(this->_dic->at(key));
//            }
    return pocket.getValue();
  }

 private:
  /**
   * @brief delete
   */
  Config();

 private:
  /**
   * @brief mutex
   */
  std::mutex _mutex;

  /**
   * @brief data structure to store config
   */
  dic_uptr _dic;
};

#define GET_CONFIG(entry) common::Config::getInstance()->get(common::DefaultConfig::entry)

}

#endif //PLAYER_CONFIG_H
