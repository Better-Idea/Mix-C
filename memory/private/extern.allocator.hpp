#undef  xuser
#define xuser mixc::memory_alloctor::inc
#include"configure/switch.hpp"
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_load.hpp"

#if xuse_libc_malloc
#include<malloc.h>

namespace mixc::memory_alloctor{
    inline uxx pused_bytes      = 0;
    inline uxx pneed_free_count = 0;

    extern voidp tiny_alloc(uxx bytes){
        inc::atom_add(xref pused_bytes, bytes);
        inc::atom_add(xref pneed_free_count, 1);

        // 按 16 字节对齐
        bytes                   = (bytes + 0xf) & ~0xf;
        return ::malloc(bytes);
    }

    extern void tiny_free(voidp ptr, uxx bytes){
        inc::atom_sub(xref pused_bytes, bytes);
        inc::atom_sub(xref pneed_free_count, 1);
        ::free(ptr);
    }
}

namespace mixc::memory_alloctor::origin{
    extern uxx used_bytes(){
        return inc::atom_load(xref pused_bytes);
    }

    extern uxx need_free_count(){
        return inc::atom_load(xref pneed_free_count);
    }

    extern uxx alive_pages(){
        auto bytes = inc::atom_load(xref pused_bytes);
        return bytes / 4096 + (bytes % 4096 != 0);
    }
}
#else

#include"memory/private/tiny_allocator.hpp"

namespace mixc::memory_alloctor{
    inline thread_local inc::tiny_allocator mem;

    extern voidp tiny_alloc(uxx bytes){
        return mem.alloc(bytes);
    }

    extern void tiny_free(voidp ptr, uxx bytes){
        mem.free(ptr, bytes);
    }
}

namespace mixc::memory_alloctor::origin{
    extern uxx used_bytes(){
        return mem.used_bytes();
    }

    extern uxx need_free_count(){
        return mem.need_free_count();
    }

    extern uxx alive_pages(){
        return mem.alive_pages();
    }
}

#endif

namespace mixc::memory_alloctor::origin{
    extern voidp malloc(size_t bytes){
        // 按 16 字节对齐
        bytes                   = (bytes + 0xf) & ~0xf;
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
