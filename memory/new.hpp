#pragma once
#ifndef _NEW
    inline void * operator new(size_t bytes, void * ptr) {
        return ptr;
    }
#endif
