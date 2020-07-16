#ifdef xuser
    #undef xuser
#endif

#define xuser mixc::memory_alloctor
#include"define/base_type.hpp"
#include"memory/private/tiny_allocator.hpp"

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
