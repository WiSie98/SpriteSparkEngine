#pragma once

#include "Sparkpch.h"

namespace SpriteSpark {

    enum LogLevel {
        Trace    = 0,
        Debug    = 1,
        Info     = 2,
        Warn     = 3,
        Error    = 4,
        Critical = 5
    };

    class Log {

    public:

        static void Init();

        inline static void SetConsoleColor(LogLevel lvl) {
#ifdef _WIN32
            static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            switch (lvl) {
            case LogLevel::Trace:
                SetConsoleTextAttribute(hConsole, 7);  // White
                break;
            case LogLevel::Debug:
                SetConsoleTextAttribute(hConsole, 11); // Cyan
                break;
            case LogLevel::Info:
                SetConsoleTextAttribute(hConsole, 10); // Green
                break;
            case LogLevel::Warn:
                SetConsoleTextAttribute(hConsole, 14); // Yellow
                break;
            case LogLevel::Error:
                SetConsoleTextAttribute(hConsole, 12); // Red
                break;
            case LogLevel::Critical:
                SetConsoleTextAttribute(hConsole, 13); // Magenta
                break;
            default:
                SetConsoleTextAttribute(hConsole, 7);  // Default to white
                break;
            }
#else
            const char* color_reset = "\033[0m";
            const char* color_trace = "\033[37m";    // White
            const char* color_debug = "\033[36m";    // Cyan
            const char* color_info = "\033[32m";     // Green
            const char* color_warn = "\033[33m";     // Yellow
            const char* color_error = "\033[31m";    // Red
            const char* color_critical = "\033[35m"; // Magenta

            switch (lvl) {
            case LogLevel::Trace:
                std::cout << color_trace;
                break;
            case LogLevel::Debug:
                std::cout << color_debug;
                break;
            case LogLevel::Info:
                std::cout << color_info;
                break;
            case LogLevel::Warn:
                std::cout << color_warn;
                break;
            case LogLevel::Error:
                std::cout << color_error;
                break;
            case LogLevel::Critical:
                std::cout << color_critical;
                break;
            default:
                std::cout << color_reset;
                break;
            }
#endif
        }

        inline static void ResetConsoleColor() {
#ifdef _WIN32
            static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 7);
#else
            const char* color_reset = "\033[0m";
            std::cout << color_reset;
#endif
        }

        template<typename... Args>
        inline static void LogMessage(LogLevel lvl, const std::string_view& str, Args&&... args) {
            std::ostringstream oss;
            oss << "[" << GetCurrentTime() << "] ";
            AppendToStream(oss, str, std::forward<Args>(args)...);

            SetConsoleColor(lvl);
            std::cout << oss.str() << std::endl;
            ResetConsoleColor();
        }

    private:

        inline static std::string GetCurrentTime() {
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm tm_now;
#ifdef _WIN32
            localtime_s(&tm_now, &now);
#else
            localtime_r(&now, &tm_now); // For Unix Systemes localtime_r gets used
#endif

            std::ostringstream oss;
            oss << std::put_time(&tm_now, "%H:%M:%S");

            return oss.str();
        }

        template<typename T>
        void static AppendToStream(std::ostringstream& oss, T&& arg) {
            oss << std::forward<T>(arg);
        }

        template<typename... Args>
        void static AppendToStream(std::ostringstream& oss, Args&&... args) {
            (oss << ... << std::forward<Args>(args));  // Fold expression to handle variadic arguments
        }
    
    };

}

// Pre-Processor definitions for stripping all log functions from distribution builds
#ifndef DIST
#define SP_CORE_TRACE(...)    ::SpriteSpark::Log::LogMessage( LogLevel::Trace, "ENGINE: ",__VA_ARGS__)
#define SP_CORE_DEBUG(...)    ::SpriteSpark::Log::LogMessage( LogLevel::Debug, "ENGINE: ",__VA_ARGS__)
#define SP_CORE_INFO(...)     ::SpriteSpark::Log::LogMessage( LogLevel::Info, "ENGINE: ",__VA_ARGS__)
#define SP_CORE_WARN(...)     ::SpriteSpark::Log::LogMessage( LogLevel::Warn, "ENGINE: ",__VA_ARGS__)
#define SP_CORE_ERROR(...)    ::SpriteSpark::Log::LogMessage( LogLevel::Error, "ENGINE: ",__VA_ARGS__)
#define SP_CORE_CRITICAL(...) ::SpriteSpark::Log::LogMessage( LogLevel::Critical, "ENGINE: ",__VA_ARGS__)

#define SP_TRACE(...)    ::SpriteSpark::Log::LogMessage( LogLevel::Trace, "APP: ",__VA_ARGS__)
#define SP_DEBUG(...)    ::SpriteSpark::Log::LogMessage( LogLevel::Debug, "APP: ",__VA_ARGS__)
#define SP_INFO(...)     ::SpriteSpark::Log::LogMessage( LogLevel::Info, "APP: ",__VA_ARGS__)
#define SP_WARN(...)     ::SpriteSpark::Log::LogMessage( LogLevel::Warn, "APP: ",__VA_ARGS__)
#define SP_ERROR(...)    ::SpriteSpark::Log::LogMessage( LogLevel::Error, "APP: ",__VA_ARGS__)
#define SP_CRITICAL(...) ::SpriteSpark::Log::LogMessage( LogLevel::Critical, "APP: ",__VA_ARGS__)
#else
#define SP_CORE_TRACE
#define SP_CORE_DEBUG
#define SP_CORE_INFO
#define SP_CORE_WARN
#define SP_CORE_ERROR
#define SP_CORE_CRITICAL

#define SP_TRACE
#define SP_DEBUG
#define SP_INFO
#define SP_WARN
#define SP_ERROR
#define SP_CRITICAL
#endif