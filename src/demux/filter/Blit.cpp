//
// Created by weihan on 2020/10/15.
//

#include "Blit.h"
#include "video/image_format.h"
#include "audio/sample_format.h"

namespace demux::filter {

    misc::vector_sptr<frame_sptr> Blit::filter(const misc::vector_sptr<frame_sptr>& in) {
        for (auto& frame : *in) {
            auto raw_frame = frame->raw();
            switch (frame->img_fmt) {
                case video::image_format::yuv420p: {
                    int ySize = raw_frame->height * raw_frame->linesize[0];
                    int uvSize = ((raw_frame->height + 1) / 2) * ((raw_frame->linesize[0] + 1) / 2);
                    int totalSize = ySize + 2 * uvSize;
                    frame->pixels = (uint8_t*)malloc(totalSize);
                    auto dst = frame->pixels;
                    memcpy(dst, raw_frame->data[0], ySize);
                    dst += ySize;
                    memcpy(dst, raw_frame->data[1], uvSize);
                    dst += uvSize;
                    memcpy(dst, raw_frame->data[2], uvSize);
                    frame->pitch = raw_frame->linesize[0];
                    frame->height = raw_frame->height;
                    break;
                }
                default: {
                    break;
                }
            }
        }
        return in;
    }

    misc::vector_sptr<frame_sptr> Blit::flush(const misc::vector_sptr<frame_sptr>& in) {
        return this->filter(in);
    }

    void Blit::close() {

    }

}
