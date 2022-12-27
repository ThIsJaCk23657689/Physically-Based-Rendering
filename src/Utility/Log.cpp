#include "Utility/Log.hpp"

#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <mutex>

void Log::ShowMe() {
    std::cout << std::endl;
    std::cout << "Created by: " << std::endl;
    std::cout << "\t\t  ________    ____         __      ________  " << std::endl;
    std::cout << "\t\t /_  __/ /_  /  _/____    / /___ _/ ____/ /__" << std::endl;
    std::cout << "\t\t  / / / __ \\ / // ___/_  / / __ `/ /   / //_/" << std::endl;
    std::cout << "\t\t / / / / / // /(__  ) /_/ / /_/ / /___/ ,<   " << std::endl;
    std::cout << "\t\t/_/ /_/ /_/___/____/\\____/\\__,_/\\____/_/|_|  " << std::endl;
    std::cout << std::endl;
}

void Log::Info(const char* fmt...) {
    char buffer[g_message_buffer_size];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buffer, std::size(buffer), fmt, args);
    va_end(args);

    std::cout << GetTimestamp() << "[INFO] " << buffer << std::endl;
}

void Log::Warning(const char* fmt...) {
    char buffer[g_message_buffer_size];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buffer, std::size(buffer), fmt, args);
    va_end(args);

    std::cout << GetTimestamp() << "[WARNING] " << buffer << std::endl;
}

void Log::Error(const char* fmt...) {
    char buffer[g_message_buffer_size];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buffer, std::size(buffer), fmt, args);
    va_end(args);

    std::cerr << GetTimestamp() << "[ERROR] " << buffer << std::endl;
}

void Log::Fatal(const char* fmt, ...) {
    char buffer[g_message_buffer_size];
    va_list args;
    va_start(args, fmt);
    std::vsnprintf(buffer, std::size(buffer), fmt, args);
    va_end(args);

    std::cerr << GetTimestamp() << "[FATAL] " << buffer << std::endl;
    std::abort();
}

std::string Log::GetTimestamp() {
    std::time_t now = std::time(0);
    std::tm tm_struct {};

#if defined(__unix__)
    locatime_r(&now, &tm_struct);
#elif defined(_MSC_VR)
    locatime_s(&tm_struct, &now);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    tm_struct = *std::localtime(&now);
#endif

    char buffer[80];
    std::strftime(buffer, std::size(buffer), "%F %T", &tm_struct);
    return buffer;
}
