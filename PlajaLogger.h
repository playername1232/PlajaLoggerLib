#ifndef PLAJA_LOGGER_PLAJALOGGER_H
#define PLAJA_LOGGER_PLAJALOGGER_H

#include <string>
#include <sstream>
#include <vector>
#include <utility>

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
private:
    enum class PlajaColor
    {
        Default,
        Red,
        Yellow,
        Blue,
        LightBlue,
        Green,
        Gray,
        White
    };

    struct PlajaLogColors
    {
        static constexpr PlajaColor Info      = PlajaColor::LightBlue;
        static constexpr PlajaColor Debug     = PlajaColor::Blue;
        static constexpr PlajaColor Warning   = PlajaColor::Yellow;
        static constexpr PlajaColor Error     = PlajaColor::Red;

        static constexpr PlajaColor Timestamp = PlajaColor::Gray;
        static constexpr PlajaColor Key       = PlajaColor::Green;
        static constexpr PlajaColor Value     = PlajaColor::White;
        static constexpr PlajaColor Separator = PlajaColor::Gray;
        static constexpr PlajaColor Message   = PlajaColor::Default;
    };

public:
    class PlajaLoggerQuery
    {
    public:
        /**
         * Appends a key-value pair to the log query.
         *
         * The provided value is converted to a string and stored for later output
         * when Write() is called.
         *
         * @param key The key associated with the appended value.
         * @param value The value to append.
         * @return A reference to the current query to allow method chaining.
         */
        template<typename TValue>
        PlajaLoggerQuery& AppendValue(const std::string& key, const TValue& value)
        {
            std::ostringstream oss;
            oss << value;

            _appends.emplace_back(key, oss.str());
            return *this;
        }

        PLAJA_LOGGER_API void Write();

    private:
        PlajaLogLevel _logLevel;
        std::string _message;
        char _appendSeparator;
        std::vector<std::pair<std::string, std::string>> _appends;

        PLAJA_LOGGER_API PlajaLoggerQuery(PlajaLogLevel logLevel, std::string message, char appendSeparator);

        [[nodiscard]] PLAJA_LOGGER_API static const char* GetLogLevelText(PlajaLogLevel logLevel);
        [[nodiscard]] PLAJA_LOGGER_API static PlajaColor GetLogLevelColor(PlajaLogLevel logLevel);

        PLAJA_LOGGER_API static void ApplyColor(PlajaColor color);
        PLAJA_LOGGER_API static void PrintColored(PlajaColor color, const std::string& text, bool canUseColors);

        friend class PlajaLogger;
    };

    /**
     * Creates a debug-level log query.
     *
     * @param message The message associated with the log entry.
     * @param appendSeparator The separator used between appended key-value pairs.
     * @return A log query that can be extended and later written using Write().
     */
    PLAJA_LOGGER_API static PlajaLoggerQuery Debug(const std::string& message, char appendSeparator = '|');

    /**
     * Creates a info-level log query.
     *
     * @param message The message associated with the log entry.
     * @param appendSeparator The separator used between appended key-value pairs.
     * @return A log query that can be extended and later written using Write().
     */
    PLAJA_LOGGER_API static PlajaLoggerQuery Info(const std::string& message, char appendSeparator = '|');

    /**
     * Creates a warning-level log query.
     *
     * @param message The message associated with the log entry.
     * @param appendSeparator The separator used between appended key-value pairs.
     * @return A log query that can be extended and later written using Write().
     */
    PLAJA_LOGGER_API static PlajaLoggerQuery Warning(const std::string& message, char appendSeparator = '|');

    /**
     * Creates a error-level log query.
     *
     * @param message The message associated with the log entry.
     * @param appendSeparator The separator used between appended key-value pairs.
     * @return A log query that can be extended and later written using Write().
     */
    PLAJA_LOGGER_API static PlajaLoggerQuery Error(const std::string& message, char appendSeparator = '|');
};

#endif