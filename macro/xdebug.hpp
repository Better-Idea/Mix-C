#pragma once
#ifdef xenable_debug_print
    #include<stdio.h>
    #include"macro/xtypeid.hpp"
    #define xdebug(token,...)         if (token) printf(__VA_ARGS__)
    #define xsw(token,...)            constexpr bool token = { __VA_ARGS__ + 0 };

    #pragma warning(push)
    #pragma GCC diagnostic ignored "-Wmacro-redefined"

    // memop/memory.hpp
    #define true    1
    xsw(im_alloc_with_initial, true);
    xsw(im_alloc, true);
    xsw(im_free, true);
    xsw(im_free_with_destroy, true);
    #undef true

    // gc/private/ref
    #define true    0
    xsw(im_inner_gc_$header, true);
    xsw(im_inner_gc_$meta, true);
    xsw(im_inner_gc_meta_routing_entry, true);
    xsw(im_inner_gc_meta_routing, true);
    xsw(im_inner_gc_meta_clear_footmark, true);

    // gc/private/tuple
    #define true    0
    xsw(im_inner_gc_tuple_routing, true);
    xsw(im_inner_gc_tuple_clear_footmark, true);
    #undef true
    #undef xsw
    #pragma warning(pop)
#else
    #define xdebug(...)
#endif
