#ifndef PLAJA_LOGGER_PLAJALOGGER_H
#define PLAJA_LOGGER_PLAJALOGGER_H

#include <string>
#include <sstream>
#include <unordered_map>

#if defined(_WIN32) || defined(_WIN64)
    #ifdef PLAJA_LOGGER_EXPORTS
        #define PLAJA_LOGGER_API __declspec(dllexport)
    #else
        #define PLAJA_LOGGER_API __declspec(dllimport)
    #endif
#else
    #define PLAJA_LOGGER_API
#endif

enum class PlajaLogLevel
{
    Debug   = 0,
    Info    = 1,
    Warning = 2,
    Error   = 3
};

class PlajaLogger
{
public:
    class PlajaLoggerQuery
    {
    public:
        template<typename TValue>
        PlajaLoggerQuery& AppendValue(const std::string& key, const TValue& value)
        {
            std::ostringstream oss;
            oss << value;

            _appends[key] = oss.str();
            return *this;
        }

        PLAJA_LOGGER_API void Write();

    private:
        PlajaLogLevel _logLevel;
        std::string _message;
        char _appendSeparator;
        std::unordered_map<std::string, std::string> _appends;

        PLAJA_LOGGER_API PlajaLoggerQuery(PlajaLogLevel logLevel, std::string message, char appendSeparator);

        friend class PlajaLogger;
    };

    PLAJA_LOGGER_API static PlajaLoggerQuery Debug(const std::string& message, char appendSeparator = '|');
    PLAJA_LOGGER_API static PlajaLoggerQuery Info(const std::string& message, char appendSeparator = '|');
    PLAJA_LOGGER_API static PlajaLoggerQuery Warning(const std::string& message, char appendSeparator = '|');
    PLAJA_LOGGER_API static PlajaLoggerQuery Error(const std::string& message, char appendSeparator = '|');
};

#endif