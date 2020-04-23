#ifndef xpack_docker_darray
#define xpack_docker_darray
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_darray
        #include"define/base_type.hpp"
        #include"dumb/dummy_t.hpp"
        #include"gc/private/ref.hpp"
        #include"interface/ranger.hpp"
        #include"macro/xgc.hpp"
        #include"memop/cast.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_darray{
        inline inc::ref_array<inc::dummy_t, inc::dummy_t> empty(inc::length(0));

        template<class type, uxx rank = 1> struct darray;
        template<class type, uxx rank>
        struct darray : public inc::ref_array<
            darray<type, rank>,
            typename darray<type, rank - 1>::the_t
        >{
            using item_t = typename darray<type, rank - 1>::the_t;
            using base_t = inc::ref_array<darray<type, rank>, item_t>;
            
            xgc_fields(
                xiam(darray<type, rank>, base_t)
            );
            
            xrange(item_t);

            darray() : 
                darray(inc::cast<the_t>(empty)) {
            }

            explicit darray(inc::length length) :
                meta(length) {}

            template<class ... args>
            explicit darray(inc::length length, args const & ... list) : 
                meta(length, list...) {}

            template<class ... args>
            darray<type> & operator()(inc::length length, args const & ... list){
                using metap = meta *;
                the.~meta();
                new (metap(this)) meta(length, list...);
                return the;
            }

            uxx length() const { 
                return meta::length(); 
            }

            item_t & operator[](uxx index){
                return meta::operator[](index);
            }

            const item_t & operator[](uxx index) const {
                return meta::operator[](index);
            }
        };

        template<class type>
        struct darray<type, 0>{
            using the_t = type;
        };
    }

#endif

namespace xuser::inc{
    using mixc::docker_darray::darray;
}
