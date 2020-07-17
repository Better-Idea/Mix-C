#ifdef xuser
    #undef xuser
#endif

#define xuser mixc::memory_alloctor
#include"define/base_type.hpp"
#include"docker/hashmap.hpp"
#include"gc/ref.hpp"
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

namespace mixc::gc_ref{
    using namespace xuser::inc;

    hashmap<visited_ptr_t, info_t>    gc_map;
    uxx                               degree_dvalue;
    visited_ptr_t                     root;
    bool                              can_free_whole_ring;
}
