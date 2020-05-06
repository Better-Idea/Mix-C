#ifndef xpack_docker_array
#define xpack_docker_array
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_array
        #include"define/base_type.hpp"
        #include"interface/ranger.hpp"
        #include"interface/seqptr.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_array{
        template<class type, uxx count = 0, uxx ... rest>
        struct array_t{
            using item_t = typename array_t<type, rest...>::the_t;
            using items  = item_t[count];

            xgc_fields(
                xiam(array_t<type, count, rest...>),
                xpro(data, items)
            );
        public:
            xranger(item_t);
            xseqptr(item_t);

            template<class ... args>
            array_t(args const & ... list) : 
                data { item_t(list)... } {}
            
            item_t & operator[] (uxx index) {
                return data[index];
            }

            const item_t & operator[] (uxx index) const {
                return data[index];
            }

            constexpr uxx length() const {
                return count;
            }

            operator item_t *(){
                return data;
            }

            operator const item_t *() const {
                return data;
            }
        };

        template<class type>
        struct array_t<type>{
            using the_t = type;
        };

        template<class final, class type, uxx count, uxx ... rest>
        struct array : array_t<type, count, rest...> {
            using the_t = array_t<type, count, rest...>;
            using the_t::the_t;
        };
    }
#endif

#include"math/index_system.hpp"

#undef  xusing_docker_array
#define xusing_docker_array     mixc::docker_array
