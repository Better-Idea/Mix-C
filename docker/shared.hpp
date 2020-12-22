#ifndef xpack_docker_shared
#define xpack_docker_shared
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_shared::inc
#include"gc/ref.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_shared{
    template<class type> struct shared;
    template<class type>
    struct shared : inc::ref_ptr<shared<type>, type>{
        using the_t  = shared<type>;
        using base_t = inc::ref_ptr<shared<type>, type>;
        using base_t::operator=;
        using base_t::operator==;
        using base_t::operator!=;
    public:
        shared() = default;

        shared(type const & value) : 
            base_t(::init_now, value){
        }

        explicit shared(::init_t) : 
            base_t(::init_now) {}

        template<class ... args>
        explicit shared(::init_t, args const & ... list) : 
            base_t(::init_now, list...) {}

        template<class ... args>
        shared<type> & operator()(::init_t, args const & ... list){
            the_t{init_now, list...}.swap(this);
            return the;
        }

        operator type & () {
            return * base_t::operator->();
        }

        operator type const & () const {
            return * base_t::operator->();
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

#endif

xexport(mixc::docker_shared::shared)
