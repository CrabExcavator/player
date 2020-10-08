//
// Created by weihan on 2020/10/8.
//

#include <glog/logging.h>

#include "Config.h"

namespace common {
    config_sptr Config::getInstance() {
        static config_sptr _config(new Config);
        return _config;
    }

    void Config::loadConfig(const std::string& path) {
        LOG(INFO) << "load config";
    }

    Config::Config(): _dic(std::make_unique<std::unordered_map<std::string, std::string>>()), _lock("config") {

    }

}
