//
// Created by weihan on 2020/10/7.
//

#include "InputContext.h"

namespace input {

    InputContext::InputContext() {
        this->_slots.reset();
    }

    InputContext::InputContext(InputContext &&rhs) noexcept: _slots(rhs._slots) {

    }

    InputContext &InputContext::operator=(InputContext &&rhs) noexcept {
        this->_slots = rhs._slots;
        return *this;
    }

    void InputContext::receive(event ev) {
        this->_slots[static_cast<int>(ev)] = true;
    }

    event_set InputContext::pollEvent() {
        auto ret = this->_slots;
        this->clear();
        return ret;
    }

    bool InputContext::hasEvent(event ev) {
        return this->_slots[static_cast<int>(ev)];
    }

    void InputContext::clear() {
        this->_slots.reset();
    }

}
