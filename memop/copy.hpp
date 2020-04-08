#ifndef xpack_memop_copy
#define xpack_memop_copy
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_copy
        #include"define/base_type.hpp"
        #include"dumb/mirror.hpp"
        #include"macro/xref.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_copy{
        template<class a, class b>
        inline void copy(a * des, b const & src){
            using mp = inc::mirror<a> *;
            *mp(des) = *mp(xref src);
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
            using mp = inc::mirror<a> *;
            copy_with_operator(mp(target), mp(source), count);
        }

        template<class a>
        inline void copy(a & target, a const & source, uxx length) {
            for(uxx i = 0; i < length; i++){
                copy(xref target[i], source[i]);
            }
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::memop_copy::copy;
    using ::mixc::memop_copy::copy_with_operator;
}
