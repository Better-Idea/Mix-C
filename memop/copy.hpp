#ifndef xpack_memop_copy
#define xpack_memop_copy
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_copy
        #include"define/base_type.hpp"
        #include"dumb/mirror.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_copy{
        template<class a, class b>
        inline void copy(a * des, b const & src){
            using m = inc::mirror<sizeof(a)>;
            ((m *)des)[0] = m(src);
        }

        template<class a>
        inline void copy_with_operator(a * target, a const * source, uxx count) {
            a * t = (a *)(target);
            a * s = (a *)(source);

            if (target > source) {
                while(count--){
                    t[count] = s[count];
                }
            }
            else {
                for (uxx index = 0; index < count; index++) {
                    t[index] = s[index];
                }
            }
        }

        template<class a>
        inline void copy(a * target, a const * source, uxx count) {
            using mp = inc::mirror<sizeof(a)> *;
            copy_with_operator(mp(target), mp(source), count);
        }
    }

#endif

namespace xuser::inc{
    using namespace mixc::memop_copy;
}
