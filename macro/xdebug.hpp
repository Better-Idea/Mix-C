#ifndef xpack_macro_xdebug
#define xpack_macro_xdebug
    #pragma warning(disable:4390)
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xdebug
        #include"configure.hpp"
        #include"macro/xtypeid.hpp"
        #include"macro/private/log.hpp"
    #pragma pop_macro("xuser")

    #if xuse_xdebug
        #define xdebug(token,...)                               \
        if (token) {                                            \
            ::mixc::macro_private_log::log(                     \
            __debug_id += 1,                                    \
            __FILE__,                                           \
            __LINE__,                                           \
            "func, " #__VA_ARGS__ ",", __func__, __VA_ARGS__);  \
        }                                                       \
        if (token)

        #define xsw(token,...)            constexpr bool token = { __VA_ARGS__ + 0 };

        inline int __debug_id;

        // docker/private/hashmap.hpp
        #define xon    0
        xsw(im_docker_hashmap_node_set                      , xon);
        xsw(im_docker_hashmap_set                           , xon);
        xsw(im_docker_hashmap_get                           , xon);
        xsw(im_docker_hashmap_addressing                    , xon);
        #undef  xon

        #define xon     1
        xsw(im_memory_classifier_take_out                   , xon);
        xsw(im_memory_classifier_free                       , xon);
        xsw(im_memory_classifier_split                      , xon);
        xsw(im_memory_classifier_append                     , xon);
        #undef  xon

        // memop/memory.hpp
        #define xon    1
        xsw(im_alloc_with_initial                           , xon);
        xsw(im_alloc                                        , xon);
        xsw(im_free                                         , xon);
        xsw(im_free_with_destroy                            , xon);
        #undef  xon

        // gc/ref
        #define xon    0
        xsw(im_gc_$token_mix                                , xon);
        xsw(im_gc_$meta                                     , xon);
        xsw(im_gc_meta_routing_entry                        , xon);
        xsw(im_gc_meta_routing                              , xon);
        #undef  xon

        // gc/private/tuple
        #define xon    0
        xsw(im_gc_tuple_routing                             , xon);
        #undef  xon

        #undef  xsw
    #else
        #define xdebug(...)
    #endif
#endif
