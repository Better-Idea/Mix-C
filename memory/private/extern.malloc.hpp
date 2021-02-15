#ifdef xuser
    #undef xuser
#endif

#define xuser mixc::memory_private_tiny_allocator::inc
#include"configure.hpp"
#include"mixc.hpp"

#include<malloc.h>

namespace mixc::memory_private_tiny_allocator::origin{
    extern voidp malloc(size_t bytes){
        return ::malloc(bytes);
    }

    extern voidp malloc_aligned(size_t bytes, size_t align_bytes){
        #if xis_windows
            return ::_mm_malloc(bytes, align_bytes);
        #endif

        #if xis_linux
            return ::memalign(align_bytes, bytes);
        #endif
    }

    extern void mfree(voidp ptr){
        ::free(ptr);
    }

    extern void mfree_aligned(voidp ptr){
        #if xis_windows
            ::_mm_free(ptr);
        #endif

        #if xis_linux
            return ::free(ptr);
        #endif
    }
}
