//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_PLAYLIST_H
#define PLAYER_PLAYLIST_H

#include <list>
#include <set>
#include <memory>

#include "PlayEntry.h"
#include "misc/typeptr.h"

namespace player {

/**
 * @brief play method define next play entry
 */
enum class play_method {
  entry_end,
  entry_loop,
  list_loop,
  list_end,
  numOfPlayMethod,
};

/**
 * @brief abstract class of different play method
 */
class PlayMethod {
 public:
  /**
   * @brief default
   */
  PlayMethod() = default;

  /**
   * @brief default
   */
  virtual ~PlayMethod() = default;

  /**
   * @brief current play entry
   * @param [in] play_list pointer to *this
   * @return current play entry
   */
  virtual play_entry_sptr current(play_list_sptr play_list) = 0;

  /**
   * @brief next play entry
   * @param [in] play_list pointer to *this
   */
  virtual void next(play_list_sptr play_list) = 0;
};

/**
 * @brief impl of entry end
 */
class PlayMethodEntryEnd : public PlayMethod {
 public:
  play_entry_sptr current(play_list_sptr play_list) override;
  void next(play_list_sptr play_list) override;
};

/**
 * @brief impl of list loop
 */
class PlayMethodListLoop : public PlayMethod {
 public:
  play_entry_sptr current(play_list_sptr play_list) override;
  void next(play_list_sptr play_list) override;
};

/**
 * @brief play list
 */
class PlayList : public std::enable_shared_from_this<PlayList> {
 public:
  // make playmethod friend
  friend class PlayMethod;
  friend class PlayMethodEntryEnd;
  friend class PlayMethodListLoop;

  /**
   * @brief default
   */
  PlayList() = default;

  /**
   * @brief default
   * @param rhs
   */
  PlayList(const PlayList &rhs) = default;

  /**
   * @brief default
   * @param rhs
   */
  PlayList(PlayList &&rhs) noexcept = default;

  /**
   * @brief default
   * @param rhs
   * @return
   */
  PlayList &operator=(const PlayList &rhs) = default;

  /**
   * @brief default
   * @param rhs
   * @return
   */
  PlayList &operator=(PlayList &&rhs) noexcept = default;

  /**
   * @brief default
   */
  ~PlayList() = default;

  /**
   * @brief add entry to last
   * @param [in] entry
   * @return pointer to *this
   */
  play_list_sptr addLast(play_entry_sptr entry);

  /**
   * @brief add entry to first
   * @param [in] entry
   * @return pointer to *this
   */
  play_list_sptr addFirst(play_entry_sptr entry);

  /**
   * @brief add entry to specified index
   * @param [in] index
   * @param [in] entry
   * @return pointer to *this
   */
  play_list_sptr addIndex(int index, play_entry_sptr entry);

  /**
   * @brief remove last entry
   * @return pointer to *this
   */
  play_list_sptr removeLast();

  /**
   * @brief remove first entry
   * @return pointer to *this
   */
  play_list_sptr removeFirst();

  /**
   * @brief remove specified entry
   * @param [in] index
   * @return pointer to *this
   */
  play_list_sptr removeIndex(int index);

  /**
   * @brief clear play list
   * @return pointer to *this
   */
  play_list_sptr clear();

  /**
   * @brief current entry
   * @return current entry
   */
  play_entry_sptr current();

  /**
   * @brief move to next entry
   */
  void next();

  /**
   * @brief number of play list
   * @return number of play list
   */
  size_t size();

  /**
   * @brief set play method
   * @param [in] method play method
   */
  void setPlayMethod(play_method method);

 private:

  /**
   * @brief if play list size == 1, change current to that entry
   */
  inline void _check_first_entry() {
    if (_entries.size() == 1) {
      _current = _entries.begin();
    }
  }

  /**
   * @brief if play list is empty, change current to null
   */
  inline void _check_empty_entry() {
    if (_entries.empty()) {
      _current = _entries.end();
    }
  }

  /**
   * @brief if current == entry, change current to null
   * @param [in] entry
   */
  inline void _check_is_current(std::list<play_entry_sptr>::iterator entry) {
    if (_current == entry) {
      _current = _entries.end();
    }
  }

 private:
  /**
   * @brief play method
   */
  std::shared_ptr<PlayMethod> _play_method;

  /**
   * @brief set is used to prevent same play entry
   */
  std::set<PlayEntry> _set;

  /**
   * @brief where to put play entry
   */
  std::list<play_entry_sptr> _entries;

  /**
   * @brief current play entry
   */
  std::list<play_entry_sptr>::iterator _current;
};
}

#endif //PLAYER_PLAYLIST_H
