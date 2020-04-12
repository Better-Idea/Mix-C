#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_strlize
#define xpack_lang_cxx_strlize
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_strlize
        #include"define/base_type.hpp"
        #include"lang/cxx.hpp"
        #include"math/numeration_t.hpp"
        #include"memory/alloc_callback.hpp"
        #include"meta/more_fit.hpp"
        #include"meta/unsigned_type.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_strlize{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            template<class type>
            auto strlize(type value, uxx base, asciis lut, inc::alloc_callback<item> alloc) const {
                constexpr uxx i = inc::more_fit<type, u08, u16, u32, u64, i08, i16, i32, i64, f32, f64>;
                u08  buf[64];
                u08p ptr = buf;

                if constexpr (i < 8){ // 是整数
                    using unsigned_t = inc::unsigned_type<type>;
                    auto u           = unsigned_t(value);
                    auto is_neg      = value < 0;

                    if (is_neg){
                        u = unsigned_t(0) - u;
                    }

                    do {
                        ptr[0] = lut[u % base];
                        ptr   += 1;
                        u     /= base;
                    } while(u != 0);

                    auto len = ptr - buf;
                    auto mem = alloc(len + is_neg);
                    auto r   = the_t(mem, len + is_neg);

                    if (is_neg){
                        mem[0] = '-';
                        mem   += 1;
                    }

                    for(uxx i = 0; i < len; i++){
                        mem[i] = buf[len - i - 1];
                    }

                    return r;
                }
                else if constexpr (i < 10){
                    // TODO: floating point =====================================================
                }
            }
        };
    }
#endif

namespace mixc::lang_cxx_strlize::xuser{
    constexpr asciis lower = "0123456789abcdefghijklmnopqrstuvwxyz";
    constexpr asciis upper = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item>{
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        #define xgen(type)                                                                      \
        cxx(type value, inc::alloc_callback<item> alloc) :                                      \
            cxx(value, inc::numeration_t::dec, lower, alloc){                                   \
        }                                                                                       \
                                                                                                \
        cxx(type value, asciis case_t, inc::alloc_callback<item> alloc) :                       \
            cxx(value, inc::numeration_t::dec, case_t, alloc){                                  \
        }                                                                                       \
                                                                                                \
        cxx(type value, inc::numeration_t base, inc::alloc_callback<item> alloc) :              \
            cxx(value, base, lower, alloc){                                                     \
        }                                                                                       \
                                                                                                \
        cxx(type value, inc::numeration_t base, asciis case_t, inc::alloc_callback<item> alloc){\
            thex = the.strlize(value, type(base), case_t, alloc);                               \
        }

        xgen(u08);
        xgen(u16);
        xgen(u32);
        xgen(u64);
        xgen(i08);
        xgen(i16);
        xgen(i32);
        xgen(i64);

        // TODO: floating point =====================================================
        #undef  xgen
    };
}

namespace xuser::inc{
    using ::mixc::lang_cxx_strlize::xuser::lower;
    using ::mixc::lang_cxx_strlize::xuser::upper;
}

#include"math/numeration_t.hpp"
#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_strlize::xuser
