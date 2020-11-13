//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "Blit.h"
#include "video/ImageFormat.h"

namespace demux::filter {

common::Error Blit::filter(const misc::vector_sptr<frame_sptr> &in, misc::vector_sptr<frame_sptr> &out) {
  for (auto &frame : *in) {
    auto raw_frame = frame->raw();
    switch (frame->img_fmt) {
      case video::ImageFormat::yuv420p: {
        int ySize = raw_frame->height * raw_frame->linesize[0];
        int uvSize = ((raw_frame->height + 1) / 2) * ((raw_frame->linesize[0] + 1) / 2);
        int totalSize = ySize + 2 * uvSize;
        frame->pixels = (uint8_t *) malloc(totalSize);
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
  out = in;
  return common::Error::SUCCESS;
}

common::Error Blit::flush(const misc::vector_sptr<frame_sptr> &in, misc::vector_sptr<frame_sptr> &out) {
  return this->filter(in, out);
}

common::Error Blit::close() {
  return common::Error::SUCCESS;
}

}
