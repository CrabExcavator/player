//
// Created by weihan on 2020/10/8.
//

#ifndef PLAYER_CONFIG_H
#define PLAYER_CONFIG_H

#include <memory>
#include <unordered_map>
#include <mutex>

#include "misc/Pocket.h"
#include "common/DefaultConfig.h"

namespace common {

    class Config;
    using config_sptr = std::shared_ptr<Config>;
    using dic_uptr = std::unique_ptr<std::unordered_map<std::string, std::string>>;

    class Config {
    public:
        static config_sptr getInstance();

        static void loadConfig(const std::string& path);

        Config(const Config& rhs) = delete;
        Config(Config&& rhs) = delete;
        Config& operator = (const Config& rhs) = delete;
        Config& operator = (Config&& rhs) = delete;
        ~Config() = default;

        template<typename T>
        T get(const misc::Pocket<T>& pocket) {
            std::lock_guard _(this->_mutex);
            const auto& key = pocket.getKey();
            // todo add additional type support
//            if (this->_dic->contains(key)) {
//                return misc::Pocket<T>::cast(this->_dic->at(key));
//            }
            return pocket.getValue();
        }

    private:
        Config();

    private:
        std::mutex _mutex;
        dic_uptr _dic;
    };

#define GET_CONFIG(entry) common::Config::getInstance()->get(common::DefaultConfig::entry)

}

#endif //PLAYER_CONFIG_H
