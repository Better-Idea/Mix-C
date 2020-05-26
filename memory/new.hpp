#pragma once
#ifndef _NEW
    inline void * operator new(size_t, void * ptr) {
        return ptr;
    }
#endif
