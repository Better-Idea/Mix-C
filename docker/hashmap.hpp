#ifdef xuser
    #ifndef xusing_docker_hashmap
        #include"docker/private/hashmap.hpp"
    #endif

    namespace xuser::inc{
        template<class key_t, class value_t>
        struct hashmap : xusing_docker_hashmap::hashmap<hashmap<key_t, value_t>, key_t, value_t>{
            using xusing_docker_hashmap::hashmap<hashmap<key_t, value_t>, key_t, value_t>::hashmap;
        };
    }
    #undef xusing_docker_hashmap
#endif
