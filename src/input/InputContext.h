//
// Created by CrabExcavator on 2020/11/15.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_INPUT_INPUTCONTEXT_H_
#define PLAYER_SRC_INPUT_INPUTCONTEXT_H_

#include <set>
#include <mutex>
#include <memory>

#include "common/Error.h"
#include "misc/typeptr.h"
#include "Event.h"

namespace input {

 class InputContext : public std::enable_shared_from_this<InputContext> {
 public:
  common::Error Init();

  void PutEvent(Event event);

  bool HasEvent(Event event);

  void DeleteEvent(Event event);

  void ClearAllEvent();

  common::Error HandleEvent(const handler::event_handler_chain_sptr& event_handler_);

 private:
  std::set<Event> events_;

  std::mutex mutex_;
};

}

#endif //PLAYER_SRC_INPUT_INPUTCONTEXT_H_
