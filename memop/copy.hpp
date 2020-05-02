#ifndef xpack_memop_copy
#define xpack_memop_copy
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_copy
        #include"define/base_type.hpp"
        #include"dumb/mirror.hpp"
        #include"memop/addressof.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_copy{
        template<class a, class b = a>
        inline void copy(a * des, b const & src){
            using mp = inc::mirror<a> *;
            *mp(des) = *mp(xref src);
        }

        template<bool with_operator, class a, class b>
        inline void copy_core(a & target, b const & source, uxx count) {
            struct itr{ 
                uxx begin; uxx end; uxx step;
            };

            itr i = xref target[0] > xref source[0] ? 
                itr{ count - 1, uxx(-1), uxx(-1) } : 
                itr{ 0, count, 1 };

            for(; i.begin != i.end; i.begin += i.step){
                if constexpr(with_operator){
                    target[i.begin] = source[i.begin];
                }
                else{
                    copy(xref target[i.begin], source[i.begin]);
                }
            }
        }

        template<class a, class b = a>
        inline void copy_with_operator(a & target, b const & source, uxx count){
            copy_core<true, a, b>(target, source, count);
        }

        template<class a, class b = a>
        inline void copy_with_operator(a && target, b const & source, uxx count){
            copy_core<true, a, b>((a &)target, source, count);
        }

        template<class a, class b = a>
        inline void copy(a & target, b const & source, uxx count) {
            copy_core<false, a, b>(target, source, count);
        }

        template<class a, class b = a>
        inline void copy(a && target, b const & source, uxx count) {
            copy_core<false, a, b>((a &)target, source, count);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::memop_copy::copy;
    using ::mixc::memop_copy::copy_with_operator;
}
