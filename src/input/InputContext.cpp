//
// Created by CrabExcavator on 2020/11/15.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "InputContext.h"
#include "misc/util.h"
#include "misc/Chain.h"

namespace input {

common::Error InputContext::Init() {
  this->events_.clear();
  return common::Error::SUCCESS;
}

void InputContext::PutEvent(Event event) {
  this->mutex_.lock();
  DEFER([&](){this->mutex_.unlock();});
  this->events_.insert(event);
}

bool InputContext::HasEvent(Event event) {
  this->mutex_.lock();
  DEFER([&](){this->mutex_.unlock();});
  return this->events_.contains(event);
}

void InputContext::DeleteEvent(Event event) {
  this->mutex_.lock();
  DEFER([&](){this->mutex_.unlock();});
  this->events_.erase(event);
}

void InputContext::ClearAllEvent() {
  this->mutex_.lock();
  DEFER([&](){this->mutex_.unlock();});
  this->events_.clear();
}
common::Error InputContext::HandleEvent(const handler::event_handler_chain_sptr& event_handler_) {
  auto in = std::make_shared<std::vector<input_ctx_sptr>>();
  in->emplace_back(shared_from_this());
  misc::vector_sptr<input_ctx_sptr> out = nullptr;
  return event_handler_->filter(in, out);
}

}