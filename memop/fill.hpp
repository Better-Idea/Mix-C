#ifndef xpack_memop_fill
#define xpack_memop_fill
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_fill
        #include"define/base_type.hpp"
        #include"memop/copy.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_fill{
        template<bool with_operator, class a, class b>
        inline void fill_core(a & target, b const & source, uxx count) {
            for (uxx i = 0; i < count; i++) {
                if constexpr(with_operator){
                    target[i] = (b &)source;
                }
                else{
                    inc::copy(xref target[i], source);
                }
            }
        }

        template<class a, class b>
        inline void fill_with_operator(a & target, b const & source, uxx count) {
            fill_core<true, a, b>(target, source, count);
        }

        template<class a, class b>
        inline void fill_with_operator(a && target, b const & source, uxx count) {
            fill_core<true, a, b>((a &)target, source, count);
        }
        
        template<class a, class b>
        inline void fill(a & target, b const & source, uxx count) {
            fill_core<false, a, b>(target, source, count);
        }
        
        template<class a, class b>
        inline void fill(a && target, b const & source, uxx count) {
            fill_core<false, a, b>((a &)target, source, count);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::memop_fill::fill;
    using ::mixc::memop_fill::fill_with_operator;
}
