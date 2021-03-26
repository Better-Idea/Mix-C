#pragma once
#include"configure/switch.hpp"
#include"define/base_type.hpp"

#if xis_msvc_native
#pragma warning(disable:4291)
#endif

inline voidp operator new(decltype(sizeof(0)), void * ptr, asciis file, uxx line) noexcept {
    return ptr;
}

#define xnew(...)           new(__VA_ARGS__, __FILE__, __LINE__)
