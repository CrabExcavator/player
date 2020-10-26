//
// Created by weihan on 2020/10/23.
//

#include <glog/logging.h>
extern "C" {
#include <libswresample/swresample.h>
}

#include "ReSample.h"

namespace demux::filter {

    misc::vector_sptr<frame_sptr> ReSample::filter(const misc::vector_sptr<frame_sptr> &in) {
        for (auto& frame : *in) {
            auto raw_frame = frame->raw();
            auto swr_ctx = swr_alloc_set_opts(
                    nullptr,
                    AV_CH_LAYOUT_STEREO,
                    AV_SAMPLE_FMT_FLT,
                    raw_frame->sample_rate,
                    raw_frame->channel_layout,
                    static_cast<AVSampleFormat>(raw_frame->format),
                    raw_frame->sample_rate,
                    0,
                    nullptr
                    );
            if (!swr_ctx) {
                // todo handle error
                LOG(INFO) << "create swr fail";
            }
            int samplessize = av_samples_get_buffer_size(nullptr,
                                                         2, raw_frame->nb_samples,
                                                         static_cast<AVSampleFormat>(raw_frame->format), 1);
            auto sambuf = (uint8_t*)av_mallocz(samplessize);
            auto samplenums = swr_convert(swr_ctx, &sambuf, samplessize,
                                          (const uint8_t**)raw_frame->extended_data, raw_frame->nb_samples);
            LOG(INFO) << "hehe";
        }
        return in;
    }

    misc::vector_sptr<frame_sptr> ReSample::flush(const misc::vector_sptr<frame_sptr> &in) {
        return this->filter(in);
    }

    void ReSample::close() {

    }

    void ReSample::init() {

    }

}
