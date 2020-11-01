//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <glog/logging.h>

#include "Config.h"

namespace core {
    config_sptr Config::getInstance() {
        static config_sptr _config(new Config);
        return _config;
    }

    void Config::loadConfig(const std::string& path) {
        LOG(INFO) << "load config";
    }

    Config::Config(): _dic(std::make_unique<std::unordered_map<std::string, std::string>>()) {

    }

}
