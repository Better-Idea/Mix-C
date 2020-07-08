#ifndef xpack_macro_private_mix
#define xpack_macro_private_mix
    #pragma GCC diagnostic ignored "-Wempty-body"
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_private_mix
    #include"define/base_type.hpp"
    #include"interface/seqptr.hpp"
    #include"meta/has_cast.hpp"
    #include"meta/more_fit.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx{
        template<class final, class item_t> struct cxx_t;
    }

    namespace mixc::macro_private_mix::origin{
        enum classify_type_t{
            is_unsigned_t,
            is_signed_t,
            is_float_t,
            is_char_t,
            is_str_t,
            is_ptr_t,
        };

        struct mix{
            union{
                f64          f;
                u64          u;
                i64          i;
                char         c;
                asciis       s;
                void *       v;
            };

            uxx              slen = not_exist;
            classify_type_t  fmt;

            template<class type>
            mix(type value){
                using result = inc::more_fit<
                    type,
                    i08,    i16,    i32,    i64,
                    u08,    u16,    u32,    u64,
                    f32,    f64,
                    asciis,
                    voidp,
                    char
                >;

                if constexpr (inc::has_cast<inc::seqptr<char>, type>){
                    s       = value;
                    slen    = value.length();
                    fmt     = is_str_t;
                }
                else if constexpr (result::index < 4){
                    i       = i64(value); // maybe is enum class
                    fmt     = is_signed_t;
                }
                else if constexpr(result::index < 8){
                    u       = u64(value);
                    fmt     = is_unsigned_t;
                }
                else if constexpr(result::index < 10){
                    f       = value;
                    fmt     = is_float_t;
                }
                else if constexpr(result::index < 11){
                    s       = value;
                    fmt     = is_str_t;
                }
                else if constexpr(result::index < 12){
                    v       = value;
                    fmt     = is_ptr_t;
                }
                else if constexpr(result::index < 13){
                    c       = value;
                    fmt     = is_char_t;
                }

                static_assert(result::index != not_exist);
            }
        };
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::macro_private_mix::origin;
}
