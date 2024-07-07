#pragma once

#include "Sparkpch.h"

#include "SparkCore/HeaderFiles/GlobalThreadPool.h"

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

        inline static void SetConsoleColor(LogLevel lvl, std::ostringstream& oss) {
#ifdef _WIN32
            static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            switch (lvl) {
            case LogLevel::Trace:
                oss << "\033[37m"; // White
                break;
            case LogLevel::Debug:
                oss << "\033[36m"; // Cyan
                break;
            case LogLevel::Info:
                oss << "\033[32m"; // Green
                break;
            case LogLevel::Warn:
                oss << "\033[33m"; // Yellow
                break;
            case LogLevel::Error:
                oss << "\033[31m"; // Red
                break;
            case LogLevel::Critical:
                oss << "\033[35m"; // Magenta
                break;
            default:
                oss << "\033[37m"; // Default to white
                break;
            }
#else
            switch (lvl) {
            case LogLevel::Trace:
                oss << "\033[37m";    // White
                break;
            case LogLevel::Debug:
                oss << "\033[36m";    // Cyan
                break;
            case LogLevel::Info:
                oss << "\033[32m";    // Green
                break;
            case LogLevel::Warn:
                oss << "\033[33m";    // Yellow
                break;
            case LogLevel::Error:
                oss << "\033[31m";    // Red
                break;
            case LogLevel::Critical:
                oss << "\033[35m";    // Magenta
                break;
            default:
                oss << "\033[0m";     // Reset
                break;
            }
#endif
        }

        inline static void ResetConsoleColor(std::ostringstream& oss) {
#ifdef _WIN32
            oss << "\033[0m";
#else
            oss << "\033[0m";
#endif
        }

        template<typename... Args>
        inline static void LogMessage(LogLevel lvl, const std::string_view& str, Args&&... args) {
            GlobalThreadPool::Get().enqueue([lvl, str, args...]() {
                std::ostringstream oss;
                SetConsoleColor(lvl, oss);
                oss << "[" << GetCurrentTime() << "] ";
                AppendToStream(oss, str, args...);
                ResetConsoleColor(oss);
                oss << std::endl;
                std::cout << oss.str();
                });
        }

    private:

        inline static std::string GetCurrentTime() {
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            std::tm tm_now;
#ifdef _WIN32
            localtime_s(&tm_now, &now);
#else
            localtime_r(&now, &tm_now);
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
            (oss << ... << std::forward<Args>(args));
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