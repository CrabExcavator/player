//
// Created by CrabExcavator on 2020/12/2.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include "DriverSDL.h"

namespace output::audio::driver {

DriverSDL::~DriverSDL() {

}

common::Error DriverSDL::Init(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  return ret;
}

common::Error DriverSDL::Open(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  return ret;
}

common::Error DriverSDL::Play(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  return ret;
}

common::Error DriverSDL::Stop(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  return ret;
}

common::Error DriverSDL::ReConfig(ao_sptr ao) {
  auto ret = common::Error::SUCCESS;
  return ret;
}

common::Error DriverSDL::GetDevices(ao_sptr ao, misc::vector_sptr<std::string> &devices) {
  auto ret = common::Error::SUCCESS;
  return ret;
}

common::Error DriverSDL::GetDesc(ao_sptr ao, tool::resample::Desc &desc) {
  auto ret = common::Error::SUCCESS;
  return ret;
}

}
