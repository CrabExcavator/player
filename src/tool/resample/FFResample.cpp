//
// Created by CrabExcavator on 2020/11/23.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "FFResample.h"

namespace tool::resample {

common::Error FFResample::Init() {
  auto ret = common::Error::SUCCESS;

  swr_ctx_uptr swr_ctx(swr_alloc(), swr_free_wrapper);
  if (nullptr == swr_ctx) {
    LOG(WARNING) << "swr alloc fail";
    ret = common::Error::UNKNOWN_ERROR;
  } else {
    swr_ctx_.swap(swr_ctx);
  }
  return ret;
}

}