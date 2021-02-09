#ifndef xpack_macro_private_mix
#define xpack_macro_private_mix
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_private_mix::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta/more_fit.hpp"
#pragma pop_macro("xuser")

namespace mixc::lang_cxx{
    template<class final, class item_t> struct cxx_t;
}

namespace mixc::macro_private_mix::inc{
    using mixc::meta_more_fit::more_fit;
}

namespace mixc::macro_private_mix::origin{
    template<class seq_type>
    concept is_asciis_seq = requires(seq_type val, asciis tmp, uxx length){
        tmp     = (asciis)val;
        length  = val.length();
    };

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
        mix(type const & value){
            using result = inc::more_fit<
                type,
                i08,    i16,    i32,    i64,
                u08,    u16,    u32,    u64,  bool,
                f32,    f64,
                asciis,
                voidp,
                char
            >;

            if constexpr (is_asciis_seq<type>){
                s       = asciis(value);
                slen    = value.length();
                fmt     = is_str_t;
            }
            else if constexpr (result::index < 4){
                i       = (i64)(type &)value; // maybe is enum class
                fmt     = is_signed_t;
            }
            else if constexpr (result::index < 9){
                u       = (u64)(type &)value;
                fmt     = is_unsigned_t;
            }
            else if constexpr (result::index < 11){
                f       = (f64)(type &)value;
                fmt     = is_float_t;
            }
            else if constexpr (result::index < 12){
                s       = (asciis)(type &)value;
                fmt     = is_str_t;
            }
            else if constexpr (result::index < 13){
                v       = (type &)value;
                fmt     = is_ptr_t;
            }
            else if constexpr (result::index < 14){
                c       = (type &)value;
                fmt     = is_char_t;
            }

            static_assert(is_asciis_seq<type> or result::index != not_exist);
        }
    };
}

#endif

xexport_space(mixc::macro_private_mix::origin)