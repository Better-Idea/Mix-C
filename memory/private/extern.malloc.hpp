#ifdef xuser
    #undef xuser
#endif

#define xuser mixc::memory_private_tiny_allocator
#include"configure.hpp"
#include"define/base_type.hpp"
#include<malloc.h>

namespace mixc::memory_private_tiny_allocator{
    voidp malloc(size_t bytes){
        return ::malloc(bytes);
    }

    voidp malloc_aligned(size_t bytes, size_t align_bytes){
        #if xis_windows
            return ::_mm_malloc(bytes, align_bytes);
        #endif

        #if xis_linux
            return ::memalign(align_bytes, bytes);
        #endif
    }

    void free(voidp ptr){
        ::free(ptr);
    }

    void free_aligned(voidp ptr){
        #if xis_windows
            ::_mm_free(ptr);
        #endif

        #if xis_linux
            return ::free(ptr);
        #endif
    }
}
