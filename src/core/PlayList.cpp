//
// Created by weihan on 2020/10/9.
//

#include "PlayList.h"

namespace core {

#define _check_set_first do{if(this->_set.contains(*entry))\
{return shared_from_this();} else{this->_set.insert(*entry);}\
}while(false)

#define _erase_set_entry(entry) do{this->_set.erase(this->_set.find(*entry));}while(false)

    play_list_sptr PlayList::addLast(play_entry_sptr entry) {
        _check_set_first;
        this->_entries.emplace_back(std::move(entry));
        this->_check_first_entry();
        return shared_from_this();
    }

    play_list_sptr PlayList::addFirst(play_entry_sptr entry) {
        _check_set_first;
        this->_entries.emplace_back(std::move(entry));
        this->_check_first_entry();
        return shared_from_this();
    }

    play_list_sptr PlayList::addIndex(int index, play_entry_sptr entry) {
        _check_set_first;
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
        this->_set.clear();
        this->_entries.clear();
        this->_check_empty_entry();
        return shared_from_this();
    }

    play_list_sptr PlayList::removeLast() {
        if (!this->_entries.empty()) {
            auto it = this->_entries.end(); it--;
            this->_check_is_current(it);
            _erase_set_entry(*it);
            this->_entries.pop_back();
        }
        this->_check_empty_entry();
        return shared_from_this();
    }

    play_list_sptr PlayList::removeFirst() {
        if (!this->_entries.empty()) {
            this->_check_is_current(this->_entries.begin());
            _erase_set_entry(*this->_entries.begin());
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
        _erase_set_entry(*it);
        this->_entries.erase(it);
        this->_check_empty_entry();
        return shared_from_this();
    }

    play_entry_sptr PlayList::current() {
        if (this->_current == this->_entries.end() && !this->_entries.empty()) {
            this->_current = this->_entries.begin();
        }
        return this->_current == this->_entries.end() ? nullptr : *this->_current;
    }

    void PlayList::next() {
        if (!this->_entries.empty()) {
            if (this->_current == this->_entries.end()) {
                this->_current = this->_entries.begin();
            } else {
                this->_current++;
                if (this->_current == this->_entries.end()) {
                    this->_current = this->_entries.begin();
                }
            }
        }
    }

    size_t PlayList::size() {
        return this->_entries.size();
    }

}
