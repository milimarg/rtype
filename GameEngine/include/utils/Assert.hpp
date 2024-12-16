#pragma once

#include <stdexcept>
#include <string>
#include <iostream>
#include <sstream>
#include <functional>
#include <map>
#include <execinfo.h>
#include <source_location>


#if __cplusplus > 202002L
    #include <stacktrace>
    #define ASSERT_ENABLE_STACKTRACE
#endif

// #ifdef NDEBUG
// #define ASSERT(cond, errorType, message, ...) ((void)0)
// #else
#define ASSERT(cond, errorType, message, ...) \
    Assert<errorType>((cond), (message), #cond, std::source_location::current(), ##__VA_ARGS__)
// #endif

// #ifdef NDEBUG
// #define ASSERT_RET(cond, errorType, message, returnValue) ((void)0)
// #else
#define ASSERT_RET(cond, errorType, message, returnValue) \
    do { \
        if (!(cond)) { \
            Assert<errorType>(false, message, #cond, std::source_location::current(), AssertMode::Log); \
            return (returnValue); \
        } \
    } while (0)
#define ASSERT_RET_L(cond, errorType, message, returnValue, lambda) \
    do { \
        if (!(cond)) { \
            Assert<errorType>(false, message, #cond, std::source_location::current(), AssertMode::Log); \
            lambda(); \
            return (returnValue); \
        } \
    } while (0)
// #endif

#define COMPILE_ASSERT(cond, message) static_assert(cond, message)

enum class AssertMode {
    Throw,
    Log,
    Exit,
    Custom
};

enum class DisplayOption {
    Condition,
    Message,
    Location,
    Function,
    StackTrace,
    Color
};

class AssertStatistics {
    public:
        static void incrementPass();
        static void incrementFail();
        static void printSummary();

    private:
        static int _passed;
        static int _failed;
};

template <typename ErrorType = std::runtime_error>
class Assert {
    public:
        struct Config {
            static bool displayCondition;
            static bool displayMessage;
            static bool displayLocation;
            static bool displayFunction;
            static bool displayStackTrace;
            static bool useColor;
        };

#ifdef ASSERT_ENABLE_STACKTRACE
        struct StackTraceConfig {
            static size_t maxFrames;
            static bool advancedDisplay;
        };
#endif

        Assert(bool condition, const std::string &message, const char *, std::source_location location = std::source_location::current(), AssertMode mode = AssertMode::Throw);

        static void setCustomHandler(const std::function<void(const std::string&)> &handler);
        static void setDisplayOptions(const std::map<DisplayOption, bool> &options);

        static void setStackTraceConfig(size_t maxFrames, bool advancedDisplay);

        Assert(const Assert&) = delete;
        Assert &operator=(const Assert&) = delete;

    private:
        static std::function<void(const std::string &)> _customHandler;

        std::string formatMessage(const char *conditionStr, const std::string &message, const std::source_location &location) const;

#ifdef ASSERT_ENABLE_STACKTRACE
        std::string getStackTrace() const;
        bool isMainFunction(const std::stacktrace_entry &frame) const;
        std::string formatFrame(const std::stacktrace_entry &frame, int frameCount) const;
        std::string formatFunction(const std::stacktrace_entry &frame, const std::string &funcName) const;
        std::string getSimpleFunctionName(const std::stacktrace_entry &frame) const;
        void appendSourceLocation(const std::stacktrace_entry &frame, std::stringstream &ss) const;
#endif

        void handleAssertion(const std::string &message, AssertMode mode) const;
};

template <typename ErrorType>
std::function<void(const std::string &)> Assert<ErrorType>::_customHandler = nullptr;

template <typename ErrorType>
bool Assert<ErrorType>::Config::displayCondition = true;
template <typename ErrorType>
bool Assert<ErrorType>::Config::displayMessage = true;
template <typename ErrorType>
bool Assert<ErrorType>::Config::displayLocation = true;
template <typename ErrorType>
bool Assert<ErrorType>::Config::displayFunction = true;
template <typename ErrorType>
bool Assert<ErrorType>::Config::useColor = false;

#ifdef ASSERT_ENABLE_STACKTRACE
    template <typename ErrorType>
    bool Assert<ErrorType>::Config::displayStackTrace = true;
    template <typename ErrorType>
    size_t Assert<ErrorType>::StackTraceConfig::maxFrames = 64;
    template <typename ErrorType>
    bool Assert<ErrorType>::StackTraceConfig::advancedDisplay = false;
#else
    template <typename ErrorType>
    bool Assert<ErrorType>::Config::displayStackTrace = false;
#endif
