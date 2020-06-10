#ifdef xuser
    #ifndef xusing_docker_bit_indicator
        #include"docker/private/bit_indicator.hpp"
    #endif

    namespace xuser::inc{
        template<uxx bits>
        struct bit_indicator : xusing_docker_bit_indicator::bit_indicator<
                bit_indicator<bits>, bits
            >{
            using xusing_docker_bit_indicator::bit_indicator<
                bit_indicator<bits>, bits
            >::bit_indicator;
        };
    }
    #undef xusing_docker_bit_indicator
#endif
