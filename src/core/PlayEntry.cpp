//
// Created by weihan on 2020/10/9.
//

#include "PlayEntry.h"

namespace core {

    PlayEntry::PlayEntry(entry_type type, std::string uri): _type(type), _uri(std::move(uri)) {

    }

}
