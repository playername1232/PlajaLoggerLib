#include "PlajaLogger.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#if defined(_WIN32) || defined(_WIN64)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

namespace
{
    bool CanUseConsoleColors()
    {
#if defined(_WIN32) || defined(_WIN64)
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if (hConsole == nullptr || hConsole == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        DWORD consoleMode = 0;
        return GetConsoleMode(hConsole, &consoleMode) != 0;
#else
        return true;
#endif
    }

    bool GetCachedCanUseConsoleColors()
    {
        static const bool canUseColors = CanUseConsoleColors();
        return canUseColors;
    }
}

// PlajaLoggerQuery

PlajaLogger::PlajaLoggerQuery::PlajaLoggerQuery(PlajaLogLevel logLevel, std::string message, char appendSeparator)
{
    _logLevel = logLevel;
    _message = std::move(message);
    _appendSeparator = appendSeparator;
}

const char* PlajaLogger::PlajaLoggerQuery::GetLogLevelText(PlajaLogLevel logLevel)
{
    switch (logLevel)
    {
        case PlajaLogLevel::Debug:
            return "DEBUG";
        case PlajaLogLevel::Info:
            return "INFO";
        case PlajaLogLevel::Warning:
            return "WARNING";
        case PlajaLogLevel::Error:
            return "ERROR";
        default:
            return "UNKNOWN";
    }
}

PlajaLogger::PlajaColor PlajaLogger::PlajaLoggerQuery::GetLogLevelColor(PlajaLogLevel logLevel)
{
    switch (logLevel)
    {
        case PlajaLogLevel::Debug:
            return PlajaLogColors::Debug;
        case PlajaLogLevel::Info:
            return PlajaLogColors::Info;
        case PlajaLogLevel::Warning:
            return PlajaLogColors::Warning;
        case PlajaLogLevel::Error:
            return PlajaLogColors::Error;
        default:
            return PlajaColor::Default;
    }
}

void PlajaLogger::PlajaLoggerQuery::ApplyColor(PlajaColor color)
{
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hConsole == nullptr || hConsole == INVALID_HANDLE_VALUE)
    {
        return;
    }

    WORD attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    switch (color)
    {
        case PlajaColor::Red:
            attr = FOREGROUND_RED | FOREGROUND_INTENSITY;
            break;
        case PlajaColor::Yellow:
            attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case PlajaColor::Blue:
            attr = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case PlajaColor::LightBlue:
            attr = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case PlajaColor::Green:
            attr = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            break;
        case PlajaColor::Gray:
            attr = FOREGROUND_INTENSITY;
            break;
        case PlajaColor::White:
            attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;
        case PlajaColor::Default:
        default:
            attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;
    }

    SetConsoleTextAttribute(hConsole, attr);
#else
    switch (color)
    {
        case PlajaColor::Red:
            std::cout << "\033[31m";
            break;
        case PlajaColor::Yellow:
            std::cout << "\033[33m";
            break;
        case PlajaColor::Blue:
            std::cout << "\033[34m";
            break;
        case PlajaColor::LightBlue:
            std::cout << "\033[36m";
            break;
        case PlajaColor::Green:
            std::cout << "\033[32m";
            break;
        case PlajaColor::Gray:
            std::cout << "\033[90m";
            break;
        case PlajaColor::White:
            std::cout << "\033[97m";
            break;
        case PlajaColor::Default:
        default:
            std::cout << "\033[0m";
            break;
    }
#endif
}

void PlajaLogger::PlajaLoggerQuery::PrintColored(PlajaColor color, const std::string& text, bool canUseColors)
{
    if (canUseColors)
    {
        ApplyColor(color);
    }

    std::cout << text;
    std::cout.flush();
}

void PlajaLogger::PlajaLoggerQuery::Write()
{
    const bool canUseColors = GetCachedCanUseConsoleColors();

    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::ostringstream timeStream;
    timeStream << "[" << std::put_time(&localTime, "%d.%m.%Y %H:%M") << "] ";

    PrintColored(PlajaLogColors::Timestamp, timeStream.str(), canUseColors);
    PrintColored(GetLogLevelColor(_logLevel), "[" + std::string(GetLogLevelText(_logLevel)) + "] ", canUseColors);
    PrintColored(PlajaLogColors::Message, _message, canUseColors);

    if (!_appends.empty())
    {
        PrintColored(PlajaLogColors::Separator, ": ", canUseColors);
    }

    bool first = true;
    for (const auto& [key, value] : _appends)
    {
        if (!first)
        {
            std::string separatorText = " ";
            separatorText += _appendSeparator;
            separatorText += " ";

            PrintColored(PlajaLogColors::Separator, separatorText, canUseColors);
        }

        PrintColored(PlajaLogColors::Key, key, canUseColors);
        PrintColored(PlajaLogColors::Separator, "=", canUseColors);
        PrintColored(PlajaLogColors::Value, value, canUseColors);

        first = false;
    }

    if (canUseColors)
    {
        ApplyColor(PlajaColor::Default);
    }

    std::cout << '\n';
}

// PlajaLogger

PlajaLogger::PlajaLoggerQuery PlajaLogger::Debug(const std::string& message, char appendSeparator)
{
    return { PlajaLogLevel::Debug, message, appendSeparator };
}

PlajaLogger::PlajaLoggerQuery PlajaLogger::Info(const std::string& message, char appendSeparator)
{
    return { PlajaLogLevel::Info, message, appendSeparator };
}

PlajaLogger::PlajaLoggerQuery PlajaLogger::Warning(const std::string& message, char appendSeparator)
{
    return { PlajaLogLevel::Warning, message, appendSeparator };
}

PlajaLogger::PlajaLoggerQuery PlajaLogger::Error(const std::string& message, char appendSeparator)
{
    return { PlajaLogLevel::Error, message, appendSeparator };
}