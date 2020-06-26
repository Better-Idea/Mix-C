#ifndef xpack_docker_shared_ptr
#define xpack_docker_shared_ptr
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::docker_shared_ptr
    #include"define/base_type.hpp"
    #include"gc/ref.hpp"
    #include"macro/xgc.hpp"

    namespace xuser::origin{
        template<class type> struct shared_ptr;
        template<class type>
        struct shared_ptr : inc::ref_ptr<shared_ptr<type>, type>{
            using base_t = inc::ref_ptr<shared_ptr<type>, type>;

            xgc_fields(
                xiam(shared_ptr<type>, base_t)
            ){
                return base_t::template routing<guide>();
            }
        public:
            shared_ptr() = default;

            explicit shared_ptr(::ini) : 
                base_t(::ini_now) {}

            template<class ... args>
            explicit shared_ptr(::ini, args const & ... list) : 
                base_t(::ini_now, list...) {}

            template<class ... args>
            shared_ptr<type> & operator()(::ini, args const & ... list){
                using metap = base_t *;
                the.~base_t();
                new (this) base_t(::length(0), list...);
                return the;
            }

            operator type & () {
                return * the.operator->();
            }

            operator type const & () const {
                return * the.operator->();
            }

            type const * operator->() const {
                return base_t::operator->();
            }

            type * operator->() {
                return base_t::operator->();
            }

            type const & operator= (type const & value){
                operator type & () = value;
                return value;
            }
        };
    }
    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace mixc::docker_shared_ptr::origin;
}
