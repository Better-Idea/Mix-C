#ifdef xuser
    #ifndef xusing_docker_darray
        #include"docker/private/darray.hpp"
    #endif

    namespace xuser::inc{
        template<class item_t, uxx rank = 1, class attribute = void>
        struct darray : 
            xusing_docker_darray::darray<
                darray<item_t, rank, attribute>, 
                item_t, 
                rank,
                attribute>{
            using xusing_docker_darray::darray<
                darray<item_t, rank, attribute>,
                item_t,
                rank,
                attribute
            >::darray;
        };
    }
    #undef xusing_docker_darray
#endif