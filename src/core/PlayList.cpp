//
// Created by weihan on 2020/10/9.
//

#include "PlayList.h"
#include "common/Config.h"

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
        if (this->_play_method == nullptr) {
            this->setPlayMethod(GET_CONFIG(default_play_method));
        }
        return this->_play_method->current(shared_from_this());
    }

    void PlayList::next() {
        if (this->_play_method == nullptr) {
            this->setPlayMethod(GET_CONFIG(default_play_method));
        }
        this->_play_method->next(shared_from_this());
    }

    size_t PlayList::size() {
        return this->_entries.size();
    }

    void PlayList::setPlayMethod(play_method method) {
        switch (method) {
            case play_method::list_loop: {
                this->_play_method = std::make_shared<PlayMethodListLoop>();
                break;
            }
            case play_method::entry_end: {
                this->_play_method = std::make_shared<PlayMethodEntryEnd>();
                break;
            }
            default:
                break;
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
