#ifndef xpack_dumb_init_by
#define xpack_dumb_init_by
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::dumb_init_by::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xnew.hpp"
#pragma pop_macro("xuser")

namespace mixc::dumb_init_by{
    template<class object_t, class ... args>
    concept can_new = requires(object_t * ptr, args const  & ... list){
        xnew(ptr) object_t(list...);
    };
}

namespace mixc::dumb_init_by::origin{

    template<class a0 = void, class ... args> struct init_by;

    template<class a0, class ... args>
    struct init_by : init_by<args...>{
        constexpr init_by(a0 const & first, args const & ... rest) :
            init_by<args...>(rest...), arg(first) {}

        template<class object_t, class ... argsx>
        void make(object_t * this_ptr, argsx const & ... list) const {
            init_by<args...>::make(this_ptr, list..., arg);
        }

        template<class object_t, class ... argsx>
        static constexpr bool test() {
            return init_by<args...>::template test<object_t, argsx..., a0>();
        }
    private:
        a0 const & arg;
    };

    template<>
    struct init_by<void>{
        constexpr init_by(){}

        template<class object_t, class ... argsx>
        void make(object_t *  this_ptr, argsx const & ... list) const {
            xnew(this_ptr) object_t(list...);
        }

        template<class object_t, class ... argsx>
        static constexpr bool test() {
            return can_new<object_t, argsx...>;
        }
    };

    constexpr init_by<> init_by_default{};

    template<class object_t, class init_by_t>
    concept can_init = init_by_t::template test<object_t>();
}

#endif

xexport_space(mixc::dumb_init_by::origin)