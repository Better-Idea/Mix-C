#pragma once
#include"configure.hpp"
#include"define/base_type.hpp"

#if xis_msvc_compatible
#pragma warning(disable:4291)
#endif

inline voidp operator new(decltype(sizeof(0)), void * ptr, asciis file, uxx line){
    return ptr;
}

#define xnew(...)           new(__VA_ARGS__, __FILE__, __LINE__)
