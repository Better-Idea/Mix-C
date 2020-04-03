#pragma once
#ifdef xenable_debug_print
    #include<stdio.h>
    #include"macro/xtypeid.hpp"
    #include"macro/private/mix.hpp"
    #define xdebug(token,...)                               \
    if (token) {                                            \
        mixc::macro_private_mix::mix::print(                \
        __debug_id += 1,                                    \
        __FILE__,                                           \
        __LINE__,                                           \
        "func, " #__VA_ARGS__ ",", __func__, __VA_ARGS__);   \
    }                                                       \
    if (token)

    #define xsw(token,...)            constexpr bool token = { __VA_ARGS__ + 0 };

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpragmas"
    #pragma GCC diagnostic ignored "-Wmacro-redefined"
    inline uxx __debug_id;

    #define xon    1
    xsw(im_docker_hashmap_set       , xon);

    // memop/memory.hpp
    #define xon    1
    xsw(im_alloc_with_initial       , xon);
    xsw(im_alloc                    , xon);
    xsw(im_free                     , xon);
    xsw(im_free_with_destroy        , xon);

    // gc/private/ref
    #define xon    0
    xsw(im_gc_$token_mix            , xon);
    xsw(im_gc_$meta                 , xon);
    xsw(im_gc_meta_routing_entry    , xon);
    xsw(im_gc_meta_routing          , xon);
    xsw(im_gc_meta_clear_footmark   , xon);

    // gc/private/tuple
    #define xon    0
    xsw(im_gc_tuple_routing         , xon);
    xsw(im_gc_tuple_clear_footmark  , xon);

    #undef xon
    #undef xsw
    #pragma GCC diagnostic pop
#else
    #define xdebug(...)
#endif
