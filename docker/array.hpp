#ifdef xuser
    #ifndef xusing_docker_array
        #include"docker/private/array.hpp"
    #endif

    namespace xuser::inc{
        template<class item, uxx count, uxx ... rest>
        struct array : 
            xusing_docker_array::array<
                array<item, count, rest...>, 
                item, count, rest...
            >{

            using xusing_docker_array::array<
                array<item, count, rest...>, 
                item, count, rest...
            >::array;
        };

        template<class type, class ... args>
        struct array_view : array<type, 1 + sizeof...(args)>{
            array_view(type const & first, args const & ... list) : 
                array<type, 1 + sizeof...(args)>(first, list...) {
            }
        };
    }
#endif
