#pragma once
#include "spdlog/spdlog.h"

#define HIPPO_DEFAULT_LOGGER_NAME "hippologger"

#if defined(HIPPO_PLATFORM_WINDOWS)
#define HIPPO_BREAK __debugbreak();
#elif defined (HIPPO_PLATFORM_MAC)
#define HIPPO_BREAK __builtin_debugtrap();
#else
#define HIPPO_BREAK __builtin_trap();
#endif

#ifndef HIPPO_CONFIG_RELEASE
#define HIPPO_TRACE(...)	if (spdlog::get(HIPPO_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(HIPPO_DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__);}
#define HIPPO_DEBUG(...)	if (spdlog::get(HIPPO_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(HIPPO_DEFAULT_LOGGER_NAME)->debug(__VA_ARGS__);}
#define HIPPO_INFO(...)		if (spdlog::get(HIPPO_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(HIPPO_DEFAULT_LOGGER_NAME)->info(__VA_ARGS__);}
#define HIPPO_WARN(...)		if (spdlog::get(HIPPO_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(HIPPO_DEFAULT_LOGGER_NAME)->warn(__VA_ARGS__);}
#define HIPPO_ERROR(...)	if (spdlog::get(HIPPO_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(HIPPO_DEFAULT_LOGGER_NAME)->error(__VA_ARGS__);}
#define HIPPO_FATAL(...)	if (spdlog::get(HIPPO_DEFAULT_LOGGER_NAME) != nullptr) {spdlog::get(HIPPO_DEFAULT_LOGGER_NAME)->critical(__VA_ARGS__);}
#define HIPPO_ASSERT(x, msg) if ((x)) {} else { HIPPO_FATAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", #x, msg, __FILE__, __LINE__); HIPPO_BREAK }
#else
// Disable logging for release builds
#define HIPPO_TRACE(...)	(void)0
#define HIPPO_DEBUG(...)	(void)0
#define HIPPO_INFO(...)		(void)0
#define HIPPO_WARN(...)		(void)0
#define HIPPO_ERROR(...)	(void)0
#define HIPPO_FATAL(...)	(void)0
#define HIPPO_ASSERT(x, msg) (void)0
#endif