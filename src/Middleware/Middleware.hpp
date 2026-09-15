#pragma once

namespace Middleware {

class Interface {
public:
    virtual ~Interface() = default;

    virtual bool begin() = 0;
    virtual bool publish(const char* channel, const char* payload) = 0;
};

} // namespace Middleware
