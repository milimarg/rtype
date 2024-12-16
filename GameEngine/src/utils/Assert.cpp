#include "Assert.hpp"
#include <cstdlib>
#include <cstring>
#include <filesystem>

#include "TestError.hpp"

int AssertStatistics::_passed = 0;
int AssertStatistics::_failed = 0;

void AssertStatistics::incrementPass()
{
    ++_passed;
}

void AssertStatistics::incrementFail()
{
    ++_failed;
}

void AssertStatistics::printSummary()
{
    std::cout << "Assertions passed: " << _passed << std::endl;
    std::cout << "Assertions failed: " << _failed << std::endl;
}

template <typename ErrorType>

Assert<ErrorType>::Assert(bool condition, const std::string &message, const char *conditionStr, std::source_location location, AssertMode mode)
{
    if (condition) {
        AssertStatistics::incrementPass();
        return;
    }

    AssertStatistics::incrementFail();
    std::string fullMessage = formatMessage(conditionStr, message, location);
    handleAssertion(fullMessage, mode);
}

template <typename ErrorType>
void Assert<ErrorType>::setCustomHandler(const std::function<void(const std::string&)> &handler)
{
    _customHandler = handler;
}

template <typename ErrorType>
void Assert<ErrorType>::setDisplayOptions(const std::map<DisplayOption, bool> &options)
{
    using enum DisplayOption;

    for (const auto& [option, value] : options) {
        switch (option) {
            case Condition: Config::displayCondition = value; break;
            case Message: Config::displayMessage = value; break;
            case Location: Config::displayLocation = value; break;
            case Function: Config::displayFunction = value; break;
            case StackTrace: Config::displayStackTrace = value; break;
            case Color: Config::useColor = value; break;
        }
    }
}

template <typename ErrorType>
void Assert<ErrorType>::setStackTraceConfig(size_t maxFrames, bool advancedDisplay)
{
#ifdef ASSERT_ENABLE_STACKTRACE
    StackTraceConfig::maxFrames = maxFrames;
    StackTraceConfig::advancedDisplay = advancedDisplay;
#else
    (void)maxFrames;
    (void)advancedDisplay;
#endif
}

template <typename ErrorType>
std::string Assert<ErrorType>::formatMessage(const char *conditionStr, const std::string &message, const std::source_location &location) const
{
    std::stringstream ss;

    if (Config::useColor)
        ss << "\033[31m";

    if (Config::displayCondition)
        ss << "Assertion failed: " << conditionStr << "\n";
    if (Config::displayMessage)
        ss << "Message: " << message << "\n";
    if (Config::displayLocation)
        ss << "At: " << location.file_name() << ":" << location.line() << ":" << location.column() << "\n";
    if (Config::displayFunction)
        ss << "In function: " << location.function_name() << "\n";
#ifdef ASSERT_ENABLE_STACKTRACE
    if (Config::displayStackTrace)
        ss << "Stack Trace:\n" << getStackTrace();
#endif
    if (Config::useColor)
        ss << "\033[0m";

    return ss.str();
}

#ifdef ASSERT_ENABLE_STACKTRACE
template <typename ErrorType>
std::string Assert<ErrorType>::getStackTrace() const
{
    auto trace = std::stacktrace::current();
    size_t offset = 3;
    const size_t maxFrames = StackTraceConfig::maxFrames;

    std::stringstream ss;
    bool stop = false;

    for (std::size_t i = offset; i < trace.size() && i < maxFrames + offset && !stop; ++i) {
        const auto& frame = trace[i];
        if (isMainFunction(frame))
            stop = true;
        ss << formatFrame(frame, i - offset);
    }

    return ss.str();
}

template <typename ErrorType>
bool Assert<ErrorType>::isMainFunction(const std::stacktrace_entry &frame) const
{
    return frame.description().find("main") != std::string::npos;
}

template <typename ErrorType>
std::string Assert<ErrorType>::formatFrame(const std::stacktrace_entry &frame, int frameCount) const
{
    std::stringstream ss;

    ss << "Frame " << frameCount << ": ";
    ss << (StackTraceConfig::advancedDisplay ? formatFunction(frame, frame.description()) : formatFunction(frame, getSimpleFunctionName(frame)));
    ss << "\n";

    return ss.str();
}

template <typename ErrorType>
std::string Assert<ErrorType>::formatFunction(const std::stacktrace_entry &frame, const std::string &funcName) const
{
    std::stringstream ss;
    ss << funcName;
    appendSourceLocation(frame, ss);
    return ss.str();
}

template <typename ErrorType>
std::string Assert<ErrorType>::getSimpleFunctionName(const std::stacktrace_entry &frame) const
{
    std::string funcName = frame.description();
    size_t pos = funcName.find_first_of('(');
    return (pos == std::string::npos) ? funcName : funcName.substr(0, pos);
}

template <typename ErrorType>
void Assert<ErrorType>::appendSourceLocation(const std::stacktrace_entry &frame, std::stringstream &ss) const
{
    if (frame.source_file().empty())
        return;

    std::filesystem::path sourcePath = std::filesystem::absolute(frame.source_file());
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path relativePath = std::filesystem::relative(sourcePath, currentPath);

    ss << " at " << relativePath << ":" << frame.source_line();
}
#endif

template <typename ErrorType>
void Assert<ErrorType>::handleAssertion(const std::string &message, AssertMode mode) const
{
    using enum AssertMode;

    switch (mode) {
        case Throw:
            throw ErrorType(message);
        case Log:
            std::cerr << message << std::endl;
            break;
        case Exit:
            std::cerr << message << std::endl;
            exit(EXIT_FAILURE);
        case Custom:
            if (_customHandler)
                _customHandler(message);
            else
                std::cerr << "Custom handler not set!" << std::endl;
            break;
    }
}

template class Assert<TestError>;
