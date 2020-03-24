#ifndef xpack_memop_fill
#define xpack_memop_fill
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_fill
        #include"define/base_type.hpp"
        #include"dumb/mirror.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_fill{
        template<class target, class source>
        inline void fill_with_operator(target * a, source const & value, uxx count) {
            for (uxx i = 0; i < count; i++) {
                a[i] = (source &)value;
            }
        }
        
        template<class target, class source>
        inline void fill(target * a, source const & value, uxx count) {
            using mp = inc::mirror<sizeof(target)> *;
            fill_with_operator(mp(a), value, count);
        }
    }

#endif

namespace xuser::inc{
    using mixc::memop_fill::fill;
    using mixc::memop_fill::fill_with_operator;
}
