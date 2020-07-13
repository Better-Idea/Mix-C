#ifdef xuser
    #ifndef xusing_docker_dbit_indicator
        #include"docker/private/dbit_indicator.hpp"
    #endif

    namespace xuser::inc{
        struct dbit_indicator : xusing_docker_dbit_indicator::dbit_indicator<
                dbit_indicator
            >{
            using xusing_docker_dbit_indicator::dbit_indicator<
                dbit_indicator
            >::dbit_indicator;
        };
    }
    #undef xusing_docker_dbit_indicator
#endif
