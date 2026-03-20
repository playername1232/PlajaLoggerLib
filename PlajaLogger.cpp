#include "PlajaLogger.h"

#include <chrono>
#include <ctime>
#include <iostream>
#include <sstream>
#include <iomanip>

// PlajaLoggerQuery

PlajaLogger::PlajaLoggerQuery::PlajaLoggerQuery(PlajaLogLevel logLevel, std::string message, char appendSeparator)
{
    _logLevel = logLevel;
    _message = std::move(message);
    _appendSeparator = appendSeparator;
}

void PlajaLogger::PlajaLoggerQuery::Write()
{
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::ostringstream oss;

    oss << "[" << std::put_time(&localTime, "%d.%m.%Y %H:%M") << "] ";

    std::string logMethod;

    switch (_logLevel)
    {
        case PlajaLogLevel::Debug:
            logMethod = "DEBUG";
            break;
        case PlajaLogLevel::Error:
            logMethod = "ERROR";
            break;
        case PlajaLogLevel::Info:
            logMethod = "INFO";
            break;
        case PlajaLogLevel::Warning:
            logMethod = "WARNING";
            break;
        default:
            logMethod = "UNKNOWN";
            break;
    }

    oss << "[" << logMethod << "] ";

    oss << _message;

    if (!_appends.empty())
        oss << ": ";

    bool first = true;
    for (const auto& [key, value] : _appends)
    {
        if (!first)
            oss << " " << _appendSeparator << " ";

        oss << key << "=" << value;

        first = false;
    }

    std::cout << oss.str() << std::endl;
}

// PlajaLogger

PlajaLogger::PlajaLoggerQuery PlajaLogger::Debug(const std::string& message, char appendSeparator)
{
    return {PlajaLogLevel::Debug, message, appendSeparator};
}

PlajaLogger::PlajaLoggerQuery PlajaLogger::Info(const std::string& message, char appendSeparator)
{
    return {PlajaLogLevel::Info, message, appendSeparator};
}

PlajaLogger::PlajaLoggerQuery PlajaLogger::Warning(const std::string& message, char appendSeparator)
{
    return {PlajaLogLevel::Warning, message, appendSeparator};
}

PlajaLogger::PlajaLoggerQuery PlajaLogger::Error(const std::string& message, char appendSeparator)
{
    return {PlajaLogLevel::Error, message, appendSeparator};
}