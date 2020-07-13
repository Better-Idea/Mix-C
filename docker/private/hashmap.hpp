#ifndef xpack_docker_hashmap
#define xpack_docker_hashmap
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::docker_hashmap
    #include"algo/hash.hpp"
    #include"define/base_type.hpp"
    #include"define/nullref.hpp"
    #include"docker/transmitter.hpp"
    #include"dumb/mirror.hpp"
    #include"dumb/disable_copy.hpp"
    #include"gc/self_management.hpp"
    #include"macro/xdebug.hpp"
    #include"macro/xstruct.hpp"
    #include"math/align.hpp"
    #include"memop/addressof.hpp"
    #include"memop/copy.hpp"
    #include"memop/swap.hpp"
    #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_hashmap{
        enum class hashmap_remove_result{
            success,
            item_not_exist,
        };

        enum class hashmap_take_out_result{
            success,
            item_not_exist,
        };

        enum class hashmap_set_result{
            success,
            override,
        };

        template<class key_t, class val_t = void>               struct node;
        template<class key_t, class val_t = void>               struct hashmap_t;
        template<class final, class key_t, class val_t = void>  struct hashmap;

        #define xarg_has_val_t
        #include"docker/private/hashmap_gen.hpp"
        #include"docker/private/hashmap_gen.hpp"
    }

    namespace mixc::docker_hashmap::origin{
        using ::mixc::docker_hashmap::hashmap_remove_result;
        using ::mixc::docker_hashmap::hashmap_take_out_result;
        using ::mixc::docker_hashmap::hashmap_set_result;
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::docker_hashmap::origin;
}

#define xusing_docker_hashmap     ::mixc::docker_hashmap
