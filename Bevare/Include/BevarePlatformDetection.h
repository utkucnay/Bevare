#pragma once

#if defined(_WIN32)
#if defined(_WIN64)

#define BEVARE_PLATFORM_WINDOWS
#define BEVARE_WITH_DX12
#define BEVARE_WITH_DX11
#define BEVARE_WITH_VULKAN

#else

#error "Win32 builds are not supported"

#endif // _WIN32

#elif defined(__APPLE__) || defined(__MACH__)

#error "Apple builds are not supported"

#elif defined(__ANDROID__)

#error "Andoid builds are not supported"

#elif defined(__Linux__)

#error "Linux is not suported"

#endif // defined(__Linux__)
