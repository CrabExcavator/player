//
// Created by weihan on 2020/10/9.
//

#include "Demuxer.h"
#include "DemuxerImpl.h"

namespace demuxer {

    Demuxer::Demuxer(): _impl() {

    }

    Demuxer::Demuxer(Demuxer &&rhs) noexcept: _impl(std::move(rhs._impl)) {
        rhs._impl = nullptr;
    }

    Demuxer &Demuxer::operator=(Demuxer &&rhs) noexcept {
        this->_impl.swap(rhs._impl);
        rhs._impl = nullptr;
        return *this;
    }

    Demuxer::~Demuxer() {
        this->_impl = nullptr;
    }

}
