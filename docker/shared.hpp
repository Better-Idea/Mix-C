#ifndef xpack_docker_shared
#define xpack_docker_shared
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_shared::inc
#include"define/base_type.hpp"
#include"dumb/init_by.hpp"
#include"gc/ref.hpp"
#include"macro/xexport.hpp"
#include"meta/has_constructor.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_shared::origin{
    using inc::init_by;
    using inc::default_init_by;

    template<class type_t> struct shared;
    template<class type_t>
    struct shared : inc::ref_ptr<shared<type_t>, type_t>{
        using the_t  = shared<type_t>;
        using base_t = inc::ref_ptr<shared<type_t>, type_t>;
        using base_t::operator==;
        using base_t::operator!=;
        using base_t::base_t;
    public:
        shared(decltype(nullptr) = nullptr) : base_t(){}
        shared(the_t const &)               = default;
        shared(the_t &&)                    = default;
        the_t & operator=(the_t const &)    = default;
        the_t & operator=(the_t &&)         = default;

        type_t & operator * () {
            return * base_t::operator->();
        }

        type_t const & operator * () const {
            return * base_t::operator->();
        }

        type_t * operator->() {
            return base_t::operator->();
        }

        type_t const * operator->() const {
            return base_t::operator->();
        }
    };
}

#endif

xexport_space(mixc::docker_shared::origin)
