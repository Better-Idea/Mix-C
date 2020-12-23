#ifndef xpack_docker_hashmap
#define xpack_docker_hashmap
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_hashmap::inc
#include"algo/hash.hpp"
#include"define/nullref.hpp"
#include"docker/transmitter.hpp"
#include"dumb/mirror.hpp"
#include"dumb/disable_copy.hpp"
#include"gc/self_management.hpp"
#include"interface/iterator.hpp"
#include"macro/xdefer.hpp"
#include"math/align.hpp"
#include"math/random.hpp"
#include"memop/addressof.hpp"
#include"memop/copy.hpp"
#include"memop/cmp.hpp"
#include"memop/swap.hpp"
#include"memory/allocator.hpp"
#include"mixc.hpp"
#include"utils/bit_indicator.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_hashmap{
    enum class hashmap_remove_result_t{
        success,
        item_not_exist,
    };

    enum class hashmap_take_out_result_t{
        success,
        item_not_exist,
    };

    enum class hashmap_set_result_t{
        success,
        override,
    };

    template<class key_t, class val_t>              struct node;
    template<class final, class key_t, class val_t> struct hashmap;

    #define xarg_has_val_t
    #include"docker/private/adapter.hashmap_gen.hpp"
    #include"docker/private/adapter.hashmap_gen.hpp"
}

namespace mixc::docker_hashmap::origin{
    using ::mixc::docker_hashmap::hashmap_remove_result_t;
    using ::mixc::docker_hashmap::hashmap_take_out_result_t;
    using ::mixc::docker_hashmap::hashmap_set_result_t;
}

#endif

#define xusing_docker_hashmap     ::mixc::docker_hashmap

xexport_space(mixc::docker_hashmap::origin)
