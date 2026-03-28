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
    private:
        PlajaLogLevel _logLevel;
        std::string _message;
        char _appendSeparator;
        char _valueHighlighter;
        std::vector<std::pair<std::string, std::string>> _appends;

        PLAJA_LOGGER_API PlajaLoggerQuery(PlajaLogLevel logLevel, std::string message, char valueHighlighter, char appendSeparator);

        PLAJA_LOGGER_API static const char* GetLogLevelText(PlajaLogLevel logLevel);
        PLAJA_LOGGER_API static PlajaColor GetLogLevelColor(PlajaLogLevel logLevel);

        PLAJA_LOGGER_API static void ApplyColor(PlajaColor color);
        PLAJA_LOGGER_API static void PrintColored(PlajaColor color, const std::string& text, bool canUseColors);

        /**
         * Extracts the value highlighter character to be used in the log query.
         *
         * If the provided highlighterOverride character is not null (`'\0'`), it will be used;
         * otherwise, the default value highlighter for the query will be returned.
         *
         * @param highlighterOverride The highlighter character to override the default, or `'\0'` to use the default value highlighter.
         * @return The character used as the value highlighter for the query.
         */
        [[nodiscard]] PLAJA_LOGGER_API char ExtractHighlighter(char highlighterOverride) const;

    public:
        /**
         * Appends a key-value pair to the log query.
         *
         * The provided value is converted to a string and stored for later output
         * when Write() is called.
         *
         * @param key The key associated with the appended value.
         * @param value The value to append.
         * @param highlighterOverride Character added in front and after the value. e.g. vH = '"' -> key="myValue"
         * @return A reference to the current query to allow method chaining.
         */
        template <typename TValue>
        [[nodiscard]] PlajaLoggerQuery& AppendValue(const std::string& key, const TValue& value, char highlighterOverride = '\0')
        {
            std::ostringstream oss;

            char realHighlighter = ExtractHighlighter(highlighterOverride);

            if (realHighlighter != '\0')
            {
                oss << realHighlighter << value << realHighlighter;
            }
            else
            {
                oss << value;
            }

            _appends.emplace_back(key, oss.str());
            return *this;
        }

        PLAJA_LOGGER_API void Write();

        friend class PlajaLogger;
    };

    /**
     * Creates a debug-level log query.
     *
     * @param message The message associated with the log entry.
     * @param appendSeparator The separator used between appended key-value pairs.
     * @return A log query that can be extended and later written using Write().
     */
    PLAJA_LOGGER_API static PlajaLoggerQuery Debug(std::string message, char valueHighlighter = '\0', char appendSeparator = '|');

    /**
     * Creates a info-level log query.
     *
     * @param message The message associated with the log entry.
     * @param appendSeparator The separator used between appended key-value pairs.
     * @return A log query that can be extended and later written using Write().
     */
    PLAJA_LOGGER_API static PlajaLoggerQuery Info(std::string message, char valueHighlighter = '\0', char appendSeparator = '|');

    /**
     * Creates a warning-level log query.
     *
     * @param message The message associated with the log entry.
     * @param appendSeparator The separator used between appended key-value pairs.
     * @return A log query that can be extended and later written using Write().
     */
    PLAJA_LOGGER_API static PlajaLoggerQuery Warning(std::string message, char valueHighlighter = '\0', char appendSeparator = '|');

    /**
     * Creates a error-level log query.
     *
     * @param message The message associated with the log entry.
     * @param appendSeparator The separator used between appended key-value pairs.
     * @return A log query that can be extended and later written using Write().
     */
    PLAJA_LOGGER_API static PlajaLoggerQuery Error(std::string message, char valueHighlighter = '\0', char appendSeparator = '|');
};

#endif