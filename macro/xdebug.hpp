#pragma once
#ifdef xenable_debug_print
    #include<stdio.h>
    #include<typeinfo>
    #define xdebug(token,...)         if (token) printf(__VA_ARGS__)
    #define xsw(token,...)            constexpr bool token = { __VA_ARGS__ + 0 };

    // memop/memory.hpp
    xsw(im_alloc_with_initial, true);
    xsw(im_alloc, true);
    xsw(im_free, true);
    xsw(im_free_with_destroy, true);

    // gc/private/ref
    xsw(im_inner_gc_$header, true);
    xsw(im_inner_gc_$meta, true);
    xsw(im_inner_gc_meta_routing_entry, true);
    xsw(im_inner_gc_meta_routing, true);
    xsw(im_inner_gc_meta_clear_footmark);

    // gc/private/tuple
    xsw(im_inner_gc_tuple_routing, true);
    xsw(im_inner_gc_tuple_clear_footmark);
    #undef xsw
#else
    #define xdebug(...)
#endif
