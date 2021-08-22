#ifndef xpack_configure_switch
#define xpack_configure_switch
#define xmixc                       "Mix-C"

#define xuse_xdebug                 0
#define xuse_xdebug_fail            1
#define xuse_xdebug_short_path      1

#define xuse_mixc_allocator         1

// 要求按 2 进制对齐
#define xgc_queue_threshold         (xgc_filter_queue_depth / 64)

// 在 xon 后面的枚举是启用 xdebug 打印的
#define xon         xlink2(__enable, __COUNTER__)       = 0

// 在 xoff 后面的枚举是禁用 xdebug 打印的
#define xoff        xlink2(__disable, __COUNTER__)      = -100000000

enum : int{
    // docker/private/hashmap.hpp
    xon,
    xoff,
        im_docker_hashmap_addressing,
        im_docker_hashmap_node_set,
        im_docker_hashmap_set,
        im_docker_hashmap_get,

    // utils/memory_flow
    xon,
    xoff,
        im_utils_tiny_allocator_free,
        im_utils_tiny_allocator_take_out,
        im_utils_tiny_allocator_split,
        im_utils_tiny_allocator_append,
        im_utils_tiny_allocator_remove,

    // memop/memory.hpp
    xon,
        im_alloc,
        im_free,
        im_alloc_with_initial,
        im_free_with_destroy,
    xoff,

    // gc/ref
    xon,
    xoff,
        im_gc_meta_routing,
        im_gc__token_mix,
        im_gc__meta,

    // gc/private/tuple
    xon,
    xoff,
        im_gc_tuple_routing,

    // 模板
    xon,
    xoff,
};

#undef  xon
#undef  xoff

#endif
