//
// Created by weihan on 2020/10/9.
//

#ifndef PLAYER_DEMUXER_H
#define PLAYER_DEMUXER_H

#include <memory>

namespace demuxer {

    class DemuxerImpl;

    class Demuxer {
    public:
        Demuxer();

    private:
        std::unique_ptr<DemuxerImpl> _impl;
    };

}
#endif //PLAYER_DEMUXER_H
