#pragma once

#include "spdlog/common.h"

class Logger
{
public:
    Logger() = delete;
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
    static void Init();

    static Logger *GetLoggerInstance()
    {
        return &sLoggerInstance;
    }

    template <typename... Args>
    void Log(spdlog::source_loc loc, spdlog::level::level_enum lvl, spdlog::format_string_t<Args...> fmt, Args &&...args)
    {
        spdlog::memory_buf_t buf;
        fmt::vformat_to(fmt::appender(buf), fmt, fmt::make_format_args(args...));
        Log(loc, lvl, buf);
    }

private:
    void Log(spdlog::source_loc loc, spdlog::level::level_enum lvl, const spdlog::memory_buf_t &buffer);

    static Logger sLoggerInstance;
};

#define LOG_LOGGER_CALL(Logger, level, ...) \
    (Logger)->Log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

#ifdef WITH_DEBUG_INFO
    #define LOG_T(...) LOG_LOGGER_CALL(Logger::GetLoggerInstance(), spdlog::level::trace, __VA_ARGS__)
    #define LOG_D(...) LOG_LOGGER_CALL(Logger::GetLoggerInstance(), spdlog::level::debug, __VA_ARGS__)
    #define LOG_I(...) LOG_LOGGER_CALL(Logger::GetLoggerInstance(), spdlog::level::info, __VA_ARGS__)
    #define LOG_W(...) LOG_LOGGER_CALL(Logger::GetLoggerInstance(), spdlog::level::warn, __VA_ARGS__)
    #define LOG_E(...) LOG_LOGGER_CALL(Logger::GetLoggerInstance(), spdlog::level::err, __VA_ARGS__)
#else
    #define LOG_T(...)
    #define LOG_D(...)
    #define LOG_I(...)
    #define LOG_W(...)
    #define LOG_E(...)
#endif  