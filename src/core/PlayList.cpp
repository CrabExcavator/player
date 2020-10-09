//
// Created by weihan on 2020/10/9.
//

#include <glog/logging.h>

#include "PlayList.h"

namespace core {

    play_list_sptr PlayList::addLast(play_entry_sptr entry) {
        this->_entries.emplace_back(std::move(entry));
        this->_check_first_entry();
        return shared_from_this();
    }

    play_list_sptr PlayList::addFirst(play_entry_sptr entry) {
        this->_entries.emplace_back(std::move(entry));
        this->_check_first_entry();
        return shared_from_this();
    }

    play_list_sptr PlayList::addIndex(int index, play_entry_sptr entry) {
        auto it = this->_entries.begin();
        int cur = 0;
        while(it != this->_entries.end()) {
            if (cur >= index) break;
            cur++; it++;
        }
        this->_entries.insert(it, std::move(entry));
        this->_check_first_entry();
        return shared_from_this();
    }

    play_list_sptr PlayList::clear() {
        this->_entries.clear();
        this->_check_empty_entry();
        return shared_from_this();
    }

    play_list_sptr PlayList::removeLast() {
        if (!this->_entries.empty()) {
            auto it = this->_entries.end(); it--;
            this->_check_is_current(it);
            this->_entries.pop_back();
        }
        this->_check_empty_entry();
        return shared_from_this();
    }

    play_list_sptr PlayList::removeFirst() {
        if (!this->_entries.empty()) {
            this->_check_is_current(this->_entries.begin());
            this->_entries.pop_front();
        }
        this->_check_empty_entry();
        return shared_from_this();
    }

    play_list_sptr PlayList::removeIndex(int index) {
        auto it = this->_entries.begin();
        int cur = 0;
        while(it != this->_entries.end()) {
            if (cur >= index) break;
            cur++; it++;
        }
        this->_check_is_current(it);
        this->_entries.erase(it);
        this->_check_empty_entry();
        return shared_from_this();
    }

}
