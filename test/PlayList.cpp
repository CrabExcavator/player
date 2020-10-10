//
// Created by weihan on 2020/10/10.
//

#include <gtest/gtest.h>

#include "misc/util.h"
#include "core/PlayList.h"
#include "core/PlayEntry.h"

TEST(PLAY_LIST, INSERT) {
    int num_of_entries = 1000;
    auto play_list = std::make_shared<core::PlayList>();
    for (int i = 0 ; i < num_of_entries ; i++) {
        play_list->addLast(std::make_shared<core::PlayEntry>(core::entry_type::file, misc::to_string(i)))
        ->addLast(std::make_shared<core::PlayEntry>(core::entry_type::file, misc::to_string(i)));
    }
    ASSERT_EQ(play_list->size(), num_of_entries);
    for (int i = 0 ; i < num_of_entries ; i++) {
        play_list->removeLast()->removeLast();
    }
    ASSERT_EQ(play_list->size(), 0);
    for (int i = 0 ; i < num_of_entries ; i++) {
        play_list->addFirst(std::make_shared<core::PlayEntry>(core::entry_type::file, misc::to_string(i)))
                ->addFirst(std::make_shared<core::PlayEntry>(core::entry_type::file, misc::to_string(i)))
                ->addFirst(std::make_shared<core::PlayEntry>(core::entry_type::file, misc::to_string(i)))
                ->addFirst(std::make_shared<core::PlayEntry>(core::entry_type::file, misc::to_string(i)))
                ->addFirst(std::make_shared<core::PlayEntry>(core::entry_type::file, misc::to_string(i)));
    }
    ASSERT_EQ(play_list->size(), num_of_entries);
    play_list->clear();
    ASSERT_EQ(play_list->size(), 0);
}

TEST(PLAY_LIST, NEXT) {
    int num_of_entries = 1000;
    auto play_list = std::make_shared<core::PlayList>();
    for (int i = 0 ; i < num_of_entries ; i++) {
        play_list->addLast(std::make_shared<core::PlayEntry>(core::entry_type::file, misc::to_string(i)))
                ->addLast(std::make_shared<core::PlayEntry>(core::entry_type::file, misc::to_string(i)));
    }
    core::play_entry_sptr last = nullptr;
    for (int i = 0 ; i < num_of_entries*10 ; i++) {
        play_list->next();
        auto current = play_list->current();
        ASSERT_NE(current, nullptr);
        ASSERT_NE(current, last);
        last = current;
    }
}
