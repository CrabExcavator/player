#include <vector>
#include <glog/logging.h>

class hehe {
public:
    hehe() {
        LOG(INFO) << "construct" << this;
    }
    ~hehe() noexcept {
        LOG(INFO) << "deconstruct" << this;
    }
    hehe(const hehe& rhs) {
        LOG(INFO) << "copy construct" << this;
    }
    hehe(hehe&& rhs)  noexcept {
        LOG(INFO) << "move construct" << this;
    }
    hehe& operator = (const hehe& rhs) {
        LOG(INFO) << "copy assignment" << this;
        return *this;
    }
    hehe& operator = (hehe&& rhs)  noexcept {
        LOG(INFO) << "move assignment" << this;
        return *this;
    }

private:
    std::unique_ptr<int> ptr;
};

class container {
public:
    container() = default;
    explicit container(hehe rhs): h(std::move(rhs)) {
    }
private:
    hehe h;
};

namespace osal {
    extern int player_main(int argc, char *argv[]);
}

int main(int argc, char* argv[]) {
    return osal::player_main(argc, argv);
}
