#ifndef xpack_docker_shared_ptr
#define xpack_docker_shared_ptr
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_shared_ptr
        #include"define/base_type.hpp"
        #include"gc/private/ref.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_shared_ptr{
        template<class type> struct shared_ptr;
        template<class type>
        xgc(shared_ptr, xpub(inc::ref_ptr<shared_ptr<type>, type>))
            using meta = inc::ref_ptr<shared_ptr<type>, type>;
            using the_t = shared_ptr<type>;

            shared_ptr() = default;

            explicit shared_ptr(inc::ini) : 
                meta(inc::length(0)) {}

            template<class ... args>
            explicit shared_ptr(inc::ini, args const & ... list) : 
                meta(inc::length(0), list...) {}


            template<class ... args>
            shared_ptr<type> & operator()(ini, args const & ... list){
                using metap = meta *;
                the.~meta();
                new (metap(this)) meta(inc::length(0), list...);
                return the;
            }

            operator type & () const {
                return meta::attr();
            }

            type const & operator= (type const & value){
                operator type & () = value;
                return value;
            }
        xgc_end();
    }

#endif

namespace xuser::inc{
    using mixc::docker_shared_ptr::shared_ptr;
}
