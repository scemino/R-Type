#pragma once

#include <spdlog/spdlog.h>

#define RTYPE_LOG_DEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define RTYPE_LOG_INFO(...) SPDLOG_INFO(__VA_ARGS__)
#define RTYPE_LOG_WARN(...) SPDLOG_WARN(__VA_ARGS__)
#define RTYPE_LOG_ERROR(...) SPDLOG_ERROR(__VA_ARGS__)
