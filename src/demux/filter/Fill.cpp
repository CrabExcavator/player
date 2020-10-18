//
// Created by weihan on 2020/10/18.
//

#include "Fill.h"
#include "demux/Stream.h"

namespace demux::filter {

    Fill::Fill(const stream_sptr& stream) {
        this->_stream = stream;
    }

    misc::vector_sptr<frame_sptr> Fill::filter(const misc::vector_sptr<frame_sptr> &in) {
        auto stream = this->_stream.lock();
        for (auto& frame : *in) {
            if (stream->_first) {
                frame->first = true;
                stream->_first = false;
            }
            frame->time_base = stream->timeBase();
            frame->pts = frame->raw()->pts;
        }
        return in;
    }

    misc::vector_sptr<frame_sptr> Fill::flush(const misc::vector_sptr<frame_sptr> &in) {
        return this->filter(in);
    }

    void Fill::close() {

    }
}
