#ifndef LOG_HPP
#define LOG_HPP

#include <string>

enum class Severity
{
    Info = 0,
    Warning,
    Error,
    Fatal
};

class Log
{
public:
    static void ShowMe();
    static void Info( const char* fmt... );
    static void Warning( const char* fmt... );
    static void Error( const char* fmt... );
    static void Fatal( const char* fmt... );

private:
    static constexpr size_t g_message_buffer_size = 4096;
    static std::string GetTimestamp();
};

#endif
