#ifndef xpack_memory_malloc
#define xpack_memory_malloc 
    #pragma push_macro("xuser")  
    #undef  xuser
    #define xuser mixc::memory_private_tiny_allocator
    #include"define/base_type.hpp"
    #include<malloc.h>

    namespace xuser{
        voidp malloc(size_t bytes){
            return ::malloc(bytes);
        }

        voidp malloc_aligned(size_t bytes, size_t align_bytes){
            return ::_mm_malloc(bytes, align_bytes);
        }

        void free(voidp ptr){
            ::free(ptr);
        }

        void free_aligned(voidp ptr){
            ::_mm_free(ptr);
        }
    }

    #pragma pop_macro("xuser")
#endif
