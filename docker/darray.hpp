#ifndef xpack_docker_darray
#define xpack_docker_darray
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_darray
        #include"define/base_type.hpp"
        #include"gc/private/ref.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_darray{
        template<class type, uxx rank = 1> struct darray;
        template<class type, uxx rank>
        xgc(darray, 
            xpub(
                inc::ref_array<
                    darray<type, rank>,
                    typename darray<type, rank - 1>::the_t
                >
            )
        )
            using item_t = typename darray<type, rank - 1>::the_t;
            using meta   = inc::ref_array<darray<type>, type>;
            using the_t  = darray<type, rank>;
            using meta::length;
            using meta::operator[];

            explicit darray(inc::length length) :
                meta(length) {}

            template<class ... args>
            explicit darray(inc::length length, args const & ... list) : 
                meta(length, list...) {}

            darray() = default;

            template<class ... args>
            darray<type> & operator()(inc::length length, args const & ... list){
                using metap = meta *;
                the.~meta();
                new (metap(this)) meta(length, list...);
                return the;
            }
        xgc_end();

        template<class type>
        struct darray<type, 0>{
            using the_t = type;
        };
    }

#endif

namespace xuser::inc{
    using mixc::docker_darray::darray;
}
