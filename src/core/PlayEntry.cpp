//
// Created by weihan on 2020/10/9.
//

#include "PlayEntry.h"

namespace core {

    PlayEntry::PlayEntry(entry_type type, std::string uri, int64_t last_pts): type(type), uri(std::move(uri)), last_pts(last_pts) {

    }

}
