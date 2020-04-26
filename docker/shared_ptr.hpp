#ifndef xpack_docker_shared_ptr
#define xpack_docker_shared_ptr
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_shared_ptr
        #include"define/base_type.hpp"
        #include"gc/ref.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_shared_ptr{
        template<class type> struct shared_ptr;
        template<class type>
        struct shared_ptr : inc::ref_ptr<shared_ptr<type>, type>{
            using base_t = inc::ref_ptr<shared_ptr<type>, type>;

            xgc_fields(
                xiam(shared_ptr<type>, base_t)
            );
        public:

            shared_ptr() = default;

            explicit shared_ptr(inc::ini) : 
                base_t(inc::ini_now) {}

            template<class ... args>
            explicit shared_ptr(inc::ini, args const & ... list) : 
                base_t(inc::ini_now, list...) {}

            template<class ... args>
            shared_ptr<type> & operator()(ini, args const & ... list){
                using metap = base_t *;
                the.~base_t();
                new (this) base_t(inc::length(0), list...);
                return the;
            }

            operator type & () const {
                return base_t::attr();
            }

            type const & operator= (type const & value){
                operator type & () = value;
                return value;
            }
        };
    }

#endif

namespace xuser::inc{
    using mixc::docker_shared_ptr::shared_ptr;
}
