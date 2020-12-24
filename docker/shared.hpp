#ifndef xpack_docker_shared
#define xpack_docker_shared
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_shared::inc
#include"gc/ref.hpp"
#include"meta/has_constructor.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_shared{
    template<class type> struct shared;
    template<class type>
    struct shared : inc::ref_ptr<shared<type>, type>{
        using the_t  = shared<type>;
        using base_t = inc::ref_ptr<shared<type>, type>;
        using base_t::operator==;
        using base_t::operator!=;
    public:
        shared(decltype(nullptr) = nullptr) : base_t(){}
        shared(the_t const &)               = default;
        shared(the_t &&)                    = default;
        the_t & operator=(the_t const &)    = default;
        the_t & operator=(the_t &&)         = default;

        template<class ... args>
        requires(inc::has_constructor<type, void(args const & ...)>)
        explicit shared(::init_t, args const & ... list) : 
            base_t(::init_now, list...) {}

        template<class ... args>
        requires(inc::has_constructor<type, void(args const & ...)>)
        shared<type> & operator()(::init_t, args const & ... list){
            the_t{init_now, list...}.swap(this);
            return the;
        }

        type & operator * () {
            return * base_t::operator->();
        }

        type const & operator * () const {
            return * base_t::operator->();
        }

        type * operator->() {
            return base_t::operator->();
        }

        type const * operator->() const {
            return base_t::operator->();
        }
    };
}

#endif

xexport(mixc::docker_shared::shared)
