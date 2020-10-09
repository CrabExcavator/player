//
// Created by weihan on 2020/10/6.
//

#include <glog/logging.h>

namespace misc {

    class init {
    public:
        init() noexcept {
            google::InitGoogleLogging("air");
#ifdef NDEBUG
            FLAGS_alsologtostderr = false;
#else
            FLAGS_alsologtostderr = true;
#endif
        }
        ~init() noexcept {
            google::ShutdownGoogleLogging();
        }
    };
}

[[maybe_unused]] static misc::init _;
