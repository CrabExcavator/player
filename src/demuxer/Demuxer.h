//
// Created by weihan on 2020/10/9.
//

#ifndef PLAYER_DEMUXER_H
#define PLAYER_DEMUXER_H

#include <memory>
#include "DemuxerImpl.h"


namespace demuxer {

    class DemuxerImpl;

    class Demuxer {
    public:
        Demuxer();
        Demuxer(const Demuxer& rhs) = delete;
        Demuxer(Demuxer&& rhs) noexcept;
        Demuxer& operator = (const Demuxer& rhs) = delete;
        Demuxer& operator = (Demuxer&& rhs) noexcept;
        ~Demuxer();

    private:
        std::unique_ptr<DemuxerImpl> _impl;
    };

}
#endif //PLAYER_DEMUXER_H
