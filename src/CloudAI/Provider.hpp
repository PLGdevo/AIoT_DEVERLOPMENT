#pragma once

namespace CloudAI {

class Provider {
public:
    virtual ~Provider() = default;

    virtual bool available() const = 0;

    // Return true when the provider successfully processes the request.
    virtual bool generate(const char* prompt, char* output, unsigned long outputSize) = 0;
};

} // namespace CloudAI
