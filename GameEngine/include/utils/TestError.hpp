#pragma once
#include <stdexcept>

class TestError : public std::exception {
    public:
        explicit TestError(const std::string &message) : _message(message) {}

        const char *what() const noexcept override
        {
            return _message.c_str();
        }

    private:
        std::string _message;
};
