#undef  xuser
#define xuser mixc::utils_memory::inc
#include"configure/switch.hpp"
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"macro/xref.hpp"

#if xuse_libc_malloc
#include<malloc.h>

namespace mixc::utils_memory{
    inline uxx g_used_bytes;
    inline uxx g_alive_object;
    inline uxx g_alive_pages;

    extern voidp tiny_alloc(uxx bytes){
        inc::atom_add(xref(g_used_bytes), bytes);
        inc::atom_add(xref(g_alive_object), 1);

        // 按 16 字节对齐
        bytes           = (bytes + 0xf) & ~0xf;
        return ::malloc(bytes);
    }

    extern void tiny_free(voidp ptr, uxx bytes){
        inc::atom_sub(xref(g_used_bytes), bytes);
        inc::atom_sub(xref(g_alive_object), 1);
        ::free(ptr);
    }
}

namespace mixc::utils_memory::origin{
    extern uxx used_bytes(){
        return inc::atom_load(xref(g_used_bytes));
    }

    extern uxx alive_object(){
        return inc::atom_load(xref(g_alive_object));
    }
}
#else

#include"utils/private/memory_flow.hpp"

namespace mixc::utils_memory{
    inline thread_local inc::memory_flow mem;

    inline uxx g_used_bytes;
    inline uxx g_alive_object;
    inline uxx g_alive_pages;

    extern voidp tiny_alloc(uxx bytes){
        return mem.alloc(bytes);
    }

    extern void tiny_free(voidp ptr, uxx bytes){
        mem.free(ptr, bytes);
    }
}

namespace mixc::utils_memory::origin{
    extern uxx used_bytes(){
        return mem.used_bytes();
    }

    extern uxx alive_object(){
        return mem.alive_object();
    }

    extern uxx alive_pages(){
        return inc::atom_load(xref(g_alive_pages));
    }
}

#endif

#include<malloc.h>
#include<windows.h>

namespace mixc::utils_memory::origin{
    extern voidp malloc(uxx bytes){
        // 按 16 字节对齐
        bytes                   = (bytes + 0xf) & ~0xf;
        return ::malloc(bytes);
    }

    extern voidp malloc_aligned(uxx bytes, uxx align_bytes){
        inc::atom_add(xref(g_alive_pages), 1);

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
        inc::atom_sub(xref(g_alive_pages), 1);

        #if xis_windows
            ::_mm_free(ptr);
        #endif

        #if xis_linux
            return ::free(ptr);
        #endif
    }
}
