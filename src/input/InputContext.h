//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_INPUTCONTEXT_H
#define PLAYER_INPUTCONTEXT_H

#include <set>
#include <memory>
#include <mutex>
#include <glog/logging.h>

#include "event.h"
#include "common/Error.h"
#include "misc/typeptr.h"
#include "handler/EventHandlerBase.h"

namespace input {

/**
 * @brief input context
 * @todo use concurrent bitset
 */
class InputContext : public std::enable_shared_from_this<InputContext> {
 public:
  friend handler::EventHandlerBase;
  /**
   * @brief default
   */
  InputContext() = default;

  /**
   * @brief delete
   * @param rhs
   */
  InputContext(const InputContext &rhs) = delete;

  /**
   * @brief copy event set
   * @param rhs
   */
  InputContext(InputContext &&rhs) noexcept;

  /**
   * @brief delete
   * @param rhs
   * @return
   */
  InputContext &operator=(const InputContext &rhs) = delete;

  /**
   * @brief copy event set
   * @param rhs
   * @return
   */
  InputContext &operator=(InputContext &&rhs) noexcept;

  /**
   * @brief default
   */
  ~InputContext() = default;

  /**
   * @brief setNumOfStream
   */
  common::Error init(const core::player_ctx_sptr &player);

  /**
   * @brief recv event
   * @param [in] ev event to set
   */
  void receiveEvent(event ev);

  /**
   * @brief judge input context have specified event
   * @param [in] ev event to judge
   * @return if has event
   */
  bool hasEvent(event ev);

  /**
   * @brief if event exist, return true and clear it, or return false
   * @param [in] ev
   * @return if event exist
   */
  bool pollEvent(event ev);

  /**
   * @brief clear event in event set
   * @param [in] ev event to clear
   */
  void clearEvent(event ev);

  /**
   * @brief clear event set
   */
  void clearAllEvent();

  /**
   * @brief handle event
   */
  common::Error handleEvent();

  /**
   * @brief next play entry, start play
   * @return error code
   */
  common::Error nextEntry();

  /**
   * @brief get current entry
   * @param [out] entry
   * @return error code
   */
  common::Error getCurrentEntry(core::play_entry_sptr &entry);

 private:
  /**
   * @brief chain to handle event
   */
  handler::event_handler_chain_sptr _handler_chain;

  /**
   * @brief weak ptr to player context
   */
  core::player_ctx_wptr _player_ctx;

  /**
   * @brief _player_list
   */
  core::play_list_sptr _player_list;

  /**
   * @brief entry to play
   * @attention must only use in Demux Context
   */
  core::play_entry_sptr _play_entry;

  /**
   * @brief events
   */
  std::set<event> _events;

  std::mutex _mutex;
};

}

#endif //PLAYER_INPUTCONTEXT_H
