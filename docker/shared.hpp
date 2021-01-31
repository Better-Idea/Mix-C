#ifndef xpack_docker_shared
#define xpack_docker_shared
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_shared::inc
#include"dumb/init_by.hpp"
#include"gc/ref.hpp"
#include"meta/has_constructor.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_shared::origin{
    using inc::init_by;
    using inc::default_init_by;

    template<class type> struct shared;
    template<class type>
    struct shared : inc::ref_ptr<shared<type>, type>{
        using the_t  = shared<type>;
        using base_t = inc::ref_ptr<shared<type>, type>;
        using base_t::operator==;
        using base_t::operator!=;
        using base_t::base_t;
    public:
        shared(decltype(nullptr) = nullptr) : base_t(){}
        shared(the_t const &)               = default;
        shared(the_t &&)                    = default;
        the_t & operator=(the_t const &)    = default;
        the_t & operator=(the_t &&)         = default;

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

xexport_space(mixc::docker_shared::origin)
