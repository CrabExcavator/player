//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_FILL_H
#define PLAYER_FILL_H

#include <memory>

#include "misc/typeptr.h"
#include "FrameFilterBase.h"

namespace demux::filter {

    /**
     * @brief init var in frame
     */
    class Fill: public FrameFilterBase {
    public:
        Fill() = delete;
        explicit Fill(const stream_sptr& stream);
        misc::vector_sptr<frame_sptr> filter(const misc::vector_sptr<frame_sptr>& in) override;
        misc::vector_sptr<frame_sptr> flush(const misc::vector_sptr<frame_sptr>& in) override;
        void close() override;

    private:
        stream_wptr _stream;
    };

}

#endif //PLAYER_FILL_H
