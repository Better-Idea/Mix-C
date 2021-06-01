#ifndef xpack_dumb_init_by
#define xpack_dumb_init_by
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::dumb_init_by::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::dumb_init_by{
    template<class object_t, class ... args_t>
    concept can_new = requires(args_t const  & ... list){
        object_t{list...};
    };
}

namespace mixc::dumb_init_by::origin{
    template<class a0_t = void, class ... args_t> struct init_by;

    template<class a0_t, class ... args_t>
    struct init_by : init_by<args_t...>{
        constexpr init_by(a0_t const & first, args_t const & ... rest) :
            init_by<args_t...>(rest...), arg(first) {}

        template<class object_t, class ... argsx_t>
        void make(object_t * this_ptr, argsx_t const & ... list) const {
            init_by<args_t...>::make(this_ptr, list..., arg);
        }

        template<class object_t, class ... argsx_t>
        static constexpr bool test() {
            return init_by<args_t...>::template test<object_t, argsx_t..., a0_t>();
        }
    private:
        a0_t const & arg;
    };

    template<>
    struct init_by<void>{
        constexpr init_by(){}

        template<class object_t, class ... argsx_t>
        void make(object_t * this_ptr, argsx_t const & ... list) const {
            xnew(this_ptr) object_t(list...);
        }

        template<class object_t, class ... argsx_t>
        static constexpr bool test() {
            return can_new<object_t, argsx_t...>;
        }
    };

    constexpr init_by<> default_init_by{};

    template<class object_t, class init_by_t>
    concept can_init = init_by_t::template test<object_t>();
}

#endif

xexport_space(mixc::dumb_init_by::origin)