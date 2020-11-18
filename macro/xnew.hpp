#pragma once
#include"define/base_type.hpp"

inline voidp operator new(decltype(sizeof(0)), void * ptr, asciis file, uxx line){
    return ptr;
}

#define xnew(...)           new(__VA_ARGS__, __FILE__, __LINE__)
