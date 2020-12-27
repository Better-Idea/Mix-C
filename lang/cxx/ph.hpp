#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_ph
#define xpack_lang_cxx_ph
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_ph::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"lang/cxx/index_of_first.hpp"
#include"lang/cxx/strlize.hpp"
#include"lang/cxx.hpp"
#include"macro/xdebug_fail.hpp"
#include"macro/xexport.hpp"
#include"memop/cast.hpp"
#include"memop/copy.hpp"
#include"memop/fill.hpp"
#include"meta/has_cast.hpp"
#include"meta/is_based_on.hpp"
#include"meta/is_origin_array.hpp"
#include"meta/is_ptr.hpp"
#include"meta/is_same.hpp"
#include"meta/item_origin_of.hpp"
#include"meta/remove_ptr.hpp"
#include"meta/unsigned_type.hpp"
#include"meta_ctr/cif.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_ph{
    /*
     * h{}  hex
     * H{}  upper hex 
     * zh{} hex with leading zero
     * zH{} upper hex with leading zero
     * x{}  hex with 0x prefix
     * X{}  upper hex with 0x prefix
     * zx{} hex with 0x prefix and leading zero
     * zX{} upper hex with 0x prefix and leading zero
     * o{}  oct
     * zo{} oct with leading zero
     * b{}  bin
     * zb{} bin with leading zero
     * 
     * v{}  generic multi-value
     * f{}  decimal(pending)
     * 
     * .l() align left
     * .r() align right
     * .c() align center
     * 
     * more... to be continue
     */

    constexpr auto place_holder_char = '\e';

    #define xfmt_specialize()                                                                   \
    inc::cxx<char> format(inc::cxx<char> fmt, inc::ialloc<char> const & alloc){                 \
        return format<char>(fmt, alloc);                                                        \
    }                                                                                           \
    inc::cxx<char16_t> format(inc::cxx<char16_t> fmt, inc::ialloc<char16_t> const & alloc){     \
        return format<char16_t>(fmt, alloc);                                                    \
    }                                                                                           \
    inc::cxx<char> format(inc::ialloc<char> const & alloc){                                     \
        return format<char>(alloc);                                                             \
    }                                                                                           \
    inc::cxx<char16_t> format(inc::ialloc<char16_t> const & alloc){                             \
        return format<char16_t>(alloc);                                                         \
    }

    xstruct(
        xname(place_holder_group)
    ) $

    template<class final, class type = void> struct base_ph;
    template<class final>
    xstruct(
        xspec(base_ph, final, void),
        xpubb(place_holder_group)
    )
    protected:
        u08         align_mode          : 2  = 0;
        u08         numeration          : 6  = 10;
        char        left_padding_char        = ' ';
        char        right_padding_char       = ' ';
        u32         align_width              = 0;

        enum{
            align_center,
            align_left,
            align_right,
        };
    public:
        final & c(uxx align_center_width, char left_padding_char, char right_padding_char){
            the.align_mode              = u08(align_center);
            the.align_width             = u32(align_center_width);
            the.left_padding_char       = left_padding_char;
            the.right_padding_char      = right_padding_char;
            return thex;
        }

        final & c(uxx align_center_width, char padding_char = ' '){
            return c(align_center_width, padding_char, padding_char);
        }

        final & l(uxx align_left_width, char padding_char = ' '){
            the.align_mode              = u08(align_left);
            the.align_width             = u32(align_left_width);
            the.right_padding_char      = padding_char;
            return thex;
        }

        final & r(uxx align_right_width, char padding_char = ' '){
            the.align_mode              = u08(align_right);
            the.align_width             = u32(align_right_width);
            the.left_padding_char       = padding_char;
            return thex;
        }

    protected:
        template<class item_t>
        item_t * align(uxx length, inc::ialloc<item_t> alloc){
            if (the_t::align_width == 0 or the_t::align_width <= length){
                return alloc(length);
            }

            auto mem       = alloc(the_t::align_width);
            auto pad_width = the_t::align_width - length;
            auto half      = pad_width / 2;

            switch(the_t::align_mode){
            case the_t::align_center:
                inc::fill_with_operator(mem, left_padding_char, half);
                inc::fill_with_operator(length + (mem += half), right_padding_char, pad_width - half);
                return mem;
            case the_t::align_left:
                inc::fill_with_operator(mem + length, right_padding_char, pad_width);
                return mem;
            default: // align_right
                inc::fill_with_operator(mem, left_padding_char, pad_width);
                return mem + pad_width;
            }
        }
    $

    template<class final, class type>
    xstruct(
        xtmpl(base_ph, final, type),
        xpubb(base_ph<final, void>),
        xprof(value, type)
    )
    public:
        constexpr base_ph(type const & value) : 
            value(value){}
    $

    template<
        class             final, 
        class             type, 
        inc::numeration_t n,
        bool              with_prefix, 
        bool              keep_leading_zero, 
        auto              lut>
    struct num : base_ph<final, type>{
        using base_t = base_ph<final, type>;
        using base_t::base_t;
        num(){}

        template<class item_t>
        item_t * format(inc::ialloc<item_t> const & alloc){
            auto buf         = (item_t *)nullptr;
            auto deformation = [this](){
                if constexpr (inc::is_ptr<type>){
                    return uxx(base_t::value);
                }
                else{
                    return base_t::value;
                }
            };

            inc::cxx<item_t>(deformation(), n, lut, [&](uxx length){
                auto klz_length = sizeof(type) * 8; // keep leading zero length

                if constexpr (n == inc::numeration_t::hex){
                    klz_length /= 4;
                }
                else if constexpr (n == inc::numeration_t::oct){
                    klz_length = klz_length / 3 + (klz_length % 3 != 0);
                }
                else if constexpr (n == inc::numeration_t::bin){
                    ; // klz_length = klz_length;
                }

                auto new_length = (keep_leading_zero ? klz_length : length);
                auto zero_count = (new_length - length);
                auto mem        = base_t::template align<item_t>(
                    new_length += (with_prefix ? 2 : 0), alloc
                );
                buf             = mem;

                if constexpr (with_prefix){ // only in hex
                    inc::copy_with_operator_unsafe(mem, "0x", 2);
                    mem        += 2;
                }
                if (zero_count){
                    inc::fill_with_operator(mem, '0', zero_count);
                    mem        += zero_count;
                }
                return mem;
            });
            return buf;
        }
    };

    constexpr bool with_prefix       = true;
    constexpr bool keep_leading_zero = true;

    #define xnum(name,equivalent_type_t,numeration,prefix,leading_zero,lut)                             \
        template<class type>                                                                            \
        struct name : base_ph<name<type>, equivalent_type_t>{                                           \
            using base_t = base_ph<name<type>, equivalent_type_t>;                                      \
            using final  = num<name<type>, equivalent_type_t, numeration, prefix, leading_zero, lut>;   \
            name(){}                                                                                    \
            name(type const & value) :                                                                  \
                base_t(equivalent_type_t(value)){                                                       \
            }                                                                                           \
            template<class item_t>                                                                      \
            item_t * format(inc::ialloc<item_t> const & alloc){                                         \
                return thex.template format<item_t>(alloc);                                             \
            }                                                                                           \
            xfmt_specialize()                                                                           \
        }
    
    #define xhex(name,prefix,leading_zero,lut)      \
        xnum(name,inc::unsigned_type<type>,inc::numeration_t::hex,prefix,leading_zero,lut)

    #define xbin(name,prefix,leading_zero,lut)      \
        xnum(name,inc::unsigned_type<type>,inc::numeration_t::bin,prefix,leading_zero,lut)

    #define xoct(name,prefix,leading_zero,lut)      \
        xnum(name,inc::unsigned_type<type>,inc::numeration_t::oct,prefix,leading_zero,lut)
}

namespace mixc::lang_cxx_ph::origin::ph{
    using mixc::lang_cxx_ph::place_holder_group;

    xhex(h , not with_prefix, not keep_leading_zero, inc::lower);
    xhex(H , not with_prefix, not keep_leading_zero, inc::upper);
    xhex(zh, not with_prefix,     keep_leading_zero, inc::lower);
    xhex(zH, not with_prefix,     keep_leading_zero, inc::upper);

    xhex(x ,     with_prefix, not keep_leading_zero, inc::lower);
    xhex(X ,     with_prefix, not keep_leading_zero, inc::upper);
    xhex(zx,     with_prefix,     keep_leading_zero, inc::lower);
    xhex(zX,     with_prefix,     keep_leading_zero, inc::upper);

    xoct(o , not with_prefix, not keep_leading_zero, inc::lower);
    xoct(zo, not with_prefix,     keep_leading_zero, inc::lower);

    xbin(b , not with_prefix, not keep_leading_zero, inc::lower);
    xbin(zb, not with_prefix,     keep_leading_zero, inc::lower);
}

namespace mixc::lang_cxx_ph{
    template<class final, class ... args> struct v;
    template<class final, class a0, class ... args>
    struct v<final, a0, args...> : v<final, args...> {
    private:
        static auto configure(){
            if constexpr (inc::is_based_on<place_holder_group, a0>){
                return (a0 *)nullptr;
            }
            else if constexpr (inc::is_origin_array<a0>){
                return (const inc::item_origin_of<a0> **)nullptr;
            }
            else if constexpr (inc::has_cast<inc::c08, a0>){
                return (inc::c08 *)nullptr;
            }
            else if constexpr (inc::is_ptr<a0>){
                return (origin::ph::zX<const void *> *)nullptr;
            }
            else{
                return (a0 *)nullptr;
            }
        }

        using val_t  = inc::remove_ptr<decltype(configure())>;
        using base_t = v<final, args...>;

        val_t item;
    public:
        constexpr v(a0 const & first, args const & ... rest) : 
            base_t(rest...), item((val_t)(a0 &)first) {
        }

        template<class item_t, class fmt_t = decltype(nullptr)>
        item_t * combine(uxx cum_length, fmt_t const & fmt = nullptr){
            item_t * ptr;
            uxx      i_e        = not_exist;

            auto link           = [&](uxx this_length){
                item_t * buf;
                fmt_t    next   = {};
                uxx      total  = cum_length + this_length;

                if constexpr (not inc::is_same<decltype(nullptr), fmt_t>){
                    if (fmt.length() != 0){
                        i_e     = fmt.index_of_first(place_holder_char);
                    }
                    if (i_e == not_exist){
                        i_e     = fmt.length();
                    }
                    else{
                        next    = fmt.backward(i_e + 1);
                    }
                }

                if (i_e == not_exist){
                    buf         = base_t::template combine<item_t>(total);
                }
                else{
                    buf         = base_t::template combine<item_t>(total + i_e, next);
                }

                buf             = buf - this_length;
                return (item_t *)buf;
            };

            if constexpr (inc::is_based_on<place_holder_group, val_t>){
                ptr             = item.template format<item_t>(link);
            }
            else{
                ptr             = inc::cxx<item_t>(item, link);
            }

            if constexpr (not inc::is_same<decltype(nullptr), fmt_t>){
                if (i_e != not_exist){
                    ptr        -= i_e;
                    inc::copy_with_operator_unsafe(ptr, inc::cxx<item_t>{fmt}.length(i_e), i_e/*length*/);
                }
            }
            return ptr;
        }
    };

    template<class final>
    struct v<final> : base_ph<final> {
        using base_t = base_ph<final>;
        using the_t  = v<final>;

        template<class item_t, class fmt_t = decltype(nullptr)>
        item_t * combine(uxx length, fmt_t const & fmt = nullptr){
            item_t * buf;
            if constexpr (not inc::is_same<decltype(nullptr), fmt_t>){
                length     += fmt.length();
                buf         = base_t::template align<item_t>(
                    length, inc::cast<inc::ialloc<item_t>>(alloc)
                ) + length;

                inc::copy_with_operator_unsafe(buf, fmt, fmt.length());
                buf        -= fmt.length();
            }
            else{
                buf         = base_t::template align<item_t>(
                    length, inc::cast<inc::ialloc<item_t>>(alloc)
                ) + length;
            }
            the.length      = length;
            return buf;
        }
    protected:
        inc::ialloc<void> alloc;
        uxx               length;
    };
}

namespace mixc::lang_cxx_ph::origin::ph{
    template<class a0, class ... args>
    struct v : mixc::lang_cxx_ph::v<v<a0, args...>, a0, args...>{
        using base_t = mixc::lang_cxx_ph::v<v<a0, args...>, a0, args...>;

        v(a0 const & first, args const & ... rest) : 
            base_t(first, rest...){}

        template<class item_t>
        inc::cxx<item_t> format(inc::cxx<item_t> fmt, inc::ialloc<item_t> const & alloc){
            inc::copy(xref base_t::alloc, alloc);
            item_t * ptr    = fmt.length() == 0 ? 
                base_t::template combine<item_t>(0) :
                base_t::template combine<item_t>(0, fmt);
            return { ptr, base_t::length };
        }

        template<class item_t>
        inc::cxx<item_t> format(inc::ialloc<item_t> const & alloc){
            inc::copy(xref base_t::alloc, alloc);
            item_t * ptr    = base_t::template combine<item_t>(0);
            return { ptr, base_t::length };
        }

        xfmt_specialize()
    };
}

#undef  xfmt_specialize
#undef  xbin
#undef  xoct
#undef  xhex
#undef  xnum
#undef  xmate
#endif

xexport_space(mixc::lang_cxx_ph::origin)