#pragma once

void* __cdecl operator new[](size_t size, const char* name, int flags, unsigned debugFlags, const char* file, int line);

void* __cdecl operator new[](unsigned __int64 size, unsigned __int64 alignment, unsigned __int64 offset, char const* pName, int flags, unsigned int debugFlags, char const* file, int line);
