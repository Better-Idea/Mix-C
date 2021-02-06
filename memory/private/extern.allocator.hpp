#ifdef xuser
    #undef xuser
#endif

#define xuser mixc::memory_alloctor::inc
#include"configure.hpp"
#include"memory/private/tiny_allocator.hpp"

#if xuse_libc_malloc
#include<malloc.h>
#include"lock/atom_add.hpp"
#include"lock/atom_sub.hpp"
namespace mixc::memory_alloctor{
    uxx pused_bytes      = 0;
    uxx pneed_free_count = 0;

    voidp malloc(uxx bytes){
        inc::atom_add(& pused_bytes, bytes);
        inc::atom_add(& pneed_free_count, uxx(1));
        return ::malloc(bytes);
    }

    void mfree(voidp ptr, uxx bytes){
        inc::atom_sub(& pused_bytes, bytes);
        inc::atom_sub(& pneed_free_count, uxx(1));
        ::free(ptr);
    }
}
namespace mixc::memory_alloctor::origin{
    uxx used_bytes(){
        return pused_bytes;
    }

    uxx need_free_count(){
        return pneed_free_count;
    }

    uxx alive_pages(){
        return pused_bytes / 4096 + (pused_bytes % 4096 != 0);
    }
}
#else

namespace mixc::memory_alloctor{
    // 单线程
    inline static inc::tiny_allocator mem;

    voidp malloc(uxx bytes){
        return mem.alloc(bytes);
    }

    void mfree(voidp ptr, uxx bytes){
        mem.free(ptr, bytes);
    }
}

namespace mixc::memory_alloctor::origin{
    uxx used_bytes(){
        return mem.used_bytes();
    }

    uxx need_free_count(){
        return mem.need_free_count();
    }

    uxx alive_pages(){
        return mem.alive_pages();
    }
}
#endif
