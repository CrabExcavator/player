//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "InputContext.h"
#include "core/PlayerContext.h"
#include "misc/Chain.h"
#include "handler/Universal.h"
#include "core/PlayList.h"
#include "misc/util.h"

namespace input {

InputContext::InputContext(InputContext &&rhs) noexcept: _events(std::move(rhs._events)) {

}

InputContext &InputContext::operator=(InputContext &&rhs) noexcept {
  this->_events = rhs._events;
  return *this;
}

common::Error InputContext::init(const core::player_ctx_sptr &player) {
  this->_player_ctx = player;
  this->_player_list = player->play_list;
  this->_handler_chain = std::make_shared<misc::Chain<input_ctx_sptr>>();
  this->_handler_chain->addLast(std::make_shared<handler::Universal>());
  return common::Error::SUCCESS;
}

void InputContext::receiveEvent(event ev) {
  this->_mutex.lock();
  DEFER([&]() {
    this->_mutex.unlock();
  });
  this->_events.insert(ev);
}

bool InputContext::hasEvent(event ev) {
  this->_mutex.lock();
  DEFER([&]() {
    this->_mutex.unlock();
  });
  return this->_events.contains(ev);
}

bool InputContext::pollEvent(event ev) {
  this->_mutex.lock();
  DEFER([&]() {
    this->_mutex.unlock();
  });
  if (this->_events.contains(ev)) {
    this->_events.erase(ev);
    return true;
  }
  return false;
}

void InputContext::clearEvent(event ev) {
  this->_mutex.lock();
  DEFER([&]() {
    this->_mutex.unlock();
  });
  this->_events.erase(ev);
}

void InputContext::clearAllEvent() {
  this->_mutex.lock();
  DEFER([&]() {
    this->_mutex.unlock();
  });
  this->_events.clear();
}

common::Error InputContext::handleEvent() {
  auto in = std::make_shared<std::vector<input_ctx_sptr>>();
  in->emplace_back(shared_from_this());
  misc::vector_sptr<input_ctx_sptr> out = nullptr;
  return this->_handler_chain->filter(in, out);
}

common::Error InputContext::nextEntry() {
  this->_play_entry = this->_player_list->current();
  this->_player_list->next();
  if (this->_play_entry != nullptr) {
    this->receiveEvent(input::event::entryAvailable);
  }
  return common::Error::SUCCESS;
}

common::Error InputContext::getCurrentEntry(core::play_entry_sptr &entry) {
  if (this->_play_entry == nullptr) {
    return common::Error::UNKNOWN_ERROR;
  }
  entry = this->_play_entry;
  this->_play_entry = nullptr;
  return common::Error::SUCCESS;
}

}
