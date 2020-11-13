//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "PlayEntry.h"

namespace core {

PlayEntry::PlayEntry(entry_type type, std::string uri, int64_t last_pts)
    : type(type), uri(std::move(uri)), last_pts(last_pts) {

}

}
