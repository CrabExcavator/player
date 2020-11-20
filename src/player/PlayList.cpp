//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "PlayList.h"
#include "common/Config.h"

namespace player {

#define _check_set_first do{if(_set.contains(*entry))\
{return shared_from_this();} else{_set.insert(*entry);}\
}while(false)

#define _erase_set_entry(entry) do{_set.erase(_set.find(*entry));}while(false)

play_list_sptr PlayList::addLast(play_entry_sptr entry) {
  _check_set_first;
  _entries.emplace_back(std::move(entry));
  _check_first_entry();
  return shared_from_this();
}

play_list_sptr PlayList::addFirst(play_entry_sptr entry) {
  _check_set_first;
  _entries.emplace_back(std::move(entry));
  _check_first_entry();
  return shared_from_this();
}

play_list_sptr PlayList::addIndex(int index, play_entry_sptr entry) {
  _check_set_first;
  auto it = _entries.begin();
  int cur = 0;
  while (it != _entries.end()) {
    if (cur >= index) break;
    cur++;
    it++;
  }
  _entries.insert(it, std::move(entry));
  _check_first_entry();
  return shared_from_this();
}

play_list_sptr PlayList::clear() {
  _set.clear();
  _entries.clear();
  _check_empty_entry();
  return shared_from_this();
}

play_list_sptr PlayList::removeLast() {
  if (!_entries.empty()) {
    auto it = _entries.end();
    it--;
    _check_is_current(it);
    _erase_set_entry(*it);
    _entries.pop_back();
  }
  _check_empty_entry();
  return shared_from_this();
}

play_list_sptr PlayList::removeFirst() {
  if (!_entries.empty()) {
    _check_is_current(_entries.begin());
    _erase_set_entry(*_entries.begin());
    _entries.pop_front();
  }
  _check_empty_entry();
  return shared_from_this();
}

play_list_sptr PlayList::removeIndex(int index) {
  auto it = _entries.begin();
  int cur = 0;
  while (it != _entries.end()) {
    if (cur >= index) break;
    cur++;
    it++;
  }
  _check_is_current(it);
  _erase_set_entry(*it);
  _entries.erase(it);
  _check_empty_entry();
  return shared_from_this();
}

play_entry_sptr PlayList::current() {
  if (_play_method == nullptr) {
    setPlayMethod(GET_CONFIG(default_play_method));
  }
  return _play_method->current(shared_from_this());
}

void PlayList::next() {
  if (_play_method == nullptr) {
    setPlayMethod(GET_CONFIG(default_play_method));
  }
  _play_method->next(shared_from_this());
}

size_t PlayList::size() {
  return _entries.size();
}

void PlayList::setPlayMethod(play_method method) {
  switch (method) {
    case play_method::list_loop: {
      _play_method = std::make_shared<PlayMethodListLoop>();
      break;
    }
    case play_method::entry_end: {
      _play_method = std::make_shared<PlayMethodEntryEnd>();
      break;
    }
    default:break;
  }
}

#define RETURN_PLAY_ENTRY return play_list->_current == play_list->_entries.end() ? nullptr : *play_list->_current

play_entry_sptr PlayMethodEntryEnd::current(play_list_sptr play_list) {
  RETURN_PLAY_ENTRY;
}

void PlayMethodEntryEnd::next(play_list_sptr play_list) {
  play_list->_current = play_list->_entries.end();
}

play_entry_sptr PlayMethodListLoop::current(play_list_sptr play_list) {
  if (play_list->_current == play_list->_entries.end() && !play_list->_entries.empty()) {
    play_list->_current = play_list->_entries.begin();
  }
  RETURN_PLAY_ENTRY;
}

void PlayMethodListLoop::next(play_list_sptr play_list) {
  if (!play_list->_entries.empty()) {
    if (play_list->_current == play_list->_entries.end()) {
      play_list->_current = play_list->_entries.begin();
    } else {
      play_list->_current++;
      if (play_list->_current == play_list->_entries.end()) {
        play_list->_current = play_list->_entries.begin();
      }
    }
  }
}
}
