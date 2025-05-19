#pragma once

#include "BevarePlatformDetection.h"

#if !defined(BEVARE_DEBUG)

#define BEVARE_CORE_LOG(...) spdlog::info(__VA_ARGS__);
#define BEVARE_CORE_WARNING(...) spdlog::warn(__VA_ARGS__);
#define BEVARE_CORE_ERROR(...) spdlog::critical(__VA_ARGS__);

#else

#define BEVARE_CORE_LOG(...)
#define BEVARE_CORE_WARNING(...)
#define BEVARE_CORE_ERROR(...)

#endif  // defined(BEVARE_DEBUG)

#if defined(BEVARE_PLATFORM_WINDOWS)

#if defined(BEVARE_SHARED_BUILD)  // TODO: Will Add CMAKE

#if defined (BEVARE_SHARED_BUILD_EXPORT)

#define BEVARE_API __declspec(dllexport)

#else

#define BEVARE_API __declspec(dllimport)

#endif  // defined(BEVARE_BUILD_DLL)

#else

#define BEVARE_API

#endif

#endif  // defined(BEVARE_PLATFORM_WINDOWS)

#define BEVARE_OFFSET(Type, Member) offsetof(Type, Member)
