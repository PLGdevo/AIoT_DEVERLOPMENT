#pragma once
#include "../Middleware.hpp"

namespace Middleware {

class ROS2 : public Interface {
public:
    bool begin() override { return false; }

    bool publish(const char* /*channel*/, const char* /*payload*/) override {
        return false;
    }
};

} // namespace Middleware
