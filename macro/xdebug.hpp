#ifndef xpack_macro_xdebug
#define xpack_macro_xdebug
    #pragma warning(disable:4390)
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_xdebug
    #include"configure.hpp"
    #include"macro/xlink.hpp"
    #include"macro/xtypeid.hpp"
    #include"macro/private/log.hpp"
    #pragma pop_macro("xuser")

    #if xuse_xdebug
        #define xdebug(token,...)                               \
        if (::token > 0 and ([&](asciis func){                  \
            ::mixc::macro_private_log::log(                     \
            __debug_id += 1,                                    \
            __FILE__,                                           \
            __LINE__,                                           \
            "func, " #__VA_ARGS__ ",", func, __VA_ARGS__);      \
            return true;                                        \
        })(__func__))

        inline int __debug_id;

        // 在 xon 后面的枚举是启用 xdebug 打印的
        #define xon         xlink2(__enable, __COUNTER__)       = 0

        // 在 xoff 后面的枚举是禁用 xdebug 打印的
        #define xoff        xlink2(__disable, __COUNTER__)      = -100000000

        enum{
            // docker/private/hashmap.hpp
            xon,
                im_docker_hashmap_node_set,
                im_docker_hashmap_set,
                im_docker_hashmap_get,
                im_docker_hashmap_addressing,
            xoff,

            // memory/tiny_allocator
            xon,
                im_memory_tiny_allocator_take_out,
                im_memory_tiny_allocator_free,
                im_memory_tiny_allocator_split,
                im_memory_tiny_allocator_append,
            xoff,

            // memop/memory.hpp
            xon,
                im_alloc_with_initial,
                im_alloc,
                im_free,
                im_free_with_destroy,
            xoff,

            // gc/ref
            xon,
                im_gc__token_mix,
                im_gc__meta,
                im_gc_meta_routing,
            xoff,

            // gc/private/tuple
            xon,
                im_gc_tuple_routing,
            xoff,

            // 模板
            xon,
            xoff,
        };

        #undef  xon
        #undef  xoff
    #else
        #define xdebug(...)
    #endif
#endif
