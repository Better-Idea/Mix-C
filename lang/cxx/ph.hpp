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

    constexpr auto place_holder_char = '\v';

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

    template<class final_t, class type = void> struct base_ph;
    template<class final_t>
    xstruct(
        xspec(base_ph, final_t, void),
        xpubb(place_holder_group)
    )
    protected:
        u08         align_mode          : 2  = 0;
        u08         numeration          : 6  = 10;
        char        left_padding_char        = ' ';
        char        right_padding_char       = ' ';
        u32         align_width              = 0;
        u32         offset_to_head           = 0;

        enum{
            align_center,
            align_left,
            align_right,
        };
    public:
        final_t & c(uxx align_center_width, char left_padding_char, char right_padding_char){
            the.align_mode              = u08(align_center);
            the.align_width             = u32(align_center_width);
            the.left_padding_char       = left_padding_char;
            the.right_padding_char      = right_padding_char;
            return thex;
        }

        final_t & c(uxx align_center_width, char padding_char = ' '){
            return c(align_center_width, padding_char, padding_char);
        }

        final_t & l(uxx align_left_width, char padding_char = ' '){
            the.align_mode              = u08(align_left);
            the.align_width             = u32(align_left_width);
            the.right_padding_char      = padding_char;
            return thex;
        }

        final_t & r(uxx align_right_width, char padding_char = ' '){
            the.align_mode              = u08(align_right);
            the.align_width             = u32(align_right_width);
            the.left_padding_char       = padding_char;
            return thex;
        }

    protected:
        template<class item_t>
        item_t * align(uxx length, uxx * cum_length_ptr, inc::ialloc<item_t> const & alloc){
            uxx     dummy               = 0;
            uxx &   cum_length          = cum_length_ptr ? cum_length_ptr[0] : dummy;

            if (the_t::align_width == 0 or the_t::align_width <= length){
                cum_length              = length;
                return alloc(length);
            }
            else{
                cum_length              = the_t::align_width;
            }

            auto mem                    = alloc(the_t::align_width);
            auto pad_width              = the_t::align_width - length;
            auto half                   = pad_width / 2;

            switch(the_t::align_mode){
            case the_t::align_center:
                inc::fill_with_operator(mem, left_padding_char, half);
                inc::fill_with_operator(length + (mem += half), right_padding_char, pad_width - half);

                // 两端填充字符用于对齐，实际返回的是居中内容缓冲区的地址
                // 要得到这个对齐块的首地址，实际上还需要减去 offset_to_head
                offset_to_head          = u32(half);
                return mem;
            case the_t::align_left:
                inc::fill_with_operator(mem + length, right_padding_char, pad_width);

                // 返回的地址就是当前对齐块的首地址
                offset_to_head          = 0;
                return mem;
            default: // align_right
                inc::fill_with_operator(mem, left_padding_char, pad_width);
                offset_to_head          = u32(pad_width);
                return mem + pad_width;
            }
        }
    $

    template<class final_t, class type>
    xstruct(
        xtmpl(base_ph, final_t, type),
        xpubb(base_ph<final_t, void>),
        xprof(value, type)
    )
    public:
        constexpr base_ph(type const & value) : 
            value(value){}
    $

    template<
        class             final_t, 
        class             type, 
        inc::numeration_t n,
        bool              with_prefix, 
        bool              keep_leading_zero, 
        auto              lut>
    struct num : base_ph<final_t, type>{
        using base_t = base_ph<final_t, type>;
        using base_t::base_t;
        num(){}

        template<class item_t>
        inc::cxx<item_t> format(inc::ialloc<item_t> const & alloc){
            auto head       = (item_t *)nullptr;
            auto total      = (uxx)0;

            inc::cxx<item_t>(deformation(), n, lut, [&](uxx length) {
                return operation<item_t>(length, xref head, xref total, alloc);
            });
            return { head - base_t::offset_to_head, total };
        }
    private:
        template<class item_t>
        item_t * operation(uxx length, item_t ** head, uxx * total, inc::ialloc<item_t> const & alloc){
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

            auto prefix_len = (with_prefix ? 2 : 0);
            auto new_len    = (keep_leading_zero ? klz_length : length) + prefix_len;
            auto zero_count = (new_len - length - prefix_len);
            auto mem        = (base_t::template align<item_t>(new_len, total, alloc));
            head[0]         = (mem);

            if constexpr (with_prefix){ // only in hex
                inc::copy_with_operator_unsafe(mem, "0x", 2);
                mem        += 2;
            }
            if (zero_count){
                inc::fill_with_operator(mem, '0', zero_count);
                mem        += zero_count;
            }
            return mem;
        }

        auto deformation() {
            if constexpr (inc::is_ptr<type>) {
                return uxx(base_t::value);
            }
            else {
                return base_t::value;
            }
        };
    };

    constexpr bool with_prefix       = true;
    constexpr bool keep_leading_zero = true;

    #define xnum(name,equivalent_type_t,numeration,prefix,leading_zero,lut)                             \
        template<class type>                                                                            \
        struct name : base_ph<name<type>, equivalent_type_t>{                                           \
            using base_t = base_ph<name<type>, equivalent_type_t>;                                      \
            using final_t  = num<name<type>, equivalent_type_t, numeration, prefix, leading_zero, lut>;   \
            name(){}                                                                                    \
            name(type const & value) :                                                                  \
                base_t(equivalent_type_t(value)){                                                       \
            }                                                                                           \
            template<class item_t>                                                                      \
            inc::cxx<item_t> format(inc::ialloc<item_t> const & alloc){                                 \
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
    template<class final_t, class ... args_t> struct v;
    template<class final_t, class a0_t, class ... args_t>
    struct v<final_t, a0_t, args_t...> : v<final_t, args_t...> {
    private:
        static auto configure(){
            if constexpr (inc::is_based_on<place_holder_group, a0_t>){
                return (a0_t *)nullptr;
            }
            else if constexpr (inc::is_origin_array<a0_t>){
                return (const inc::item_origin_of<a0_t> **)nullptr;
            }
            else if constexpr (inc::has_cast<inc::c08, a0_t>){
                return (inc::c08 *)nullptr;
            }
            else if constexpr (inc::is_ptr<a0_t>){
                return (origin::ph::zX<const void *> *)nullptr;
            }
            else{
                return (a0_t *)nullptr;
            }
        }

        using val_t  = inc::remove_ptr<decltype(configure())>;
        using base_t = v<final_t, args_t...>;

        val_t item;
    public:
        constexpr v(a0_t const & first, args_t const & ... rest) : 
            base_t(rest...), item((val_t)(a0_t &)first) {
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

                if (base_t * base_t = this; i_e == not_exist) {
                    buf         = base_t->template combine<item_t>(total);
                }
                else{
                    buf         = base_t->template combine<item_t>(total + i_e, next);
                }

                buf             = buf - this_length;
                return (item_t *)buf;
            };

            if constexpr (inc::is_based_on<place_holder_group, val_t>){
                ptr             = (item_t *)item.template format<item_t>(link);
            }
            else{
                ptr             = (item_t *)inc::cxx<item_t>(item, link);
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

    template<class final_t>
    struct v<final_t> : base_ph<final_t> {
        using base_t = base_ph<final_t>;
        using the_t  = v<final_t>;

        template<class item_t, class fmt_t = decltype(nullptr)>
        item_t * combine(uxx length, fmt_t const & fmt = nullptr){
            item_t * buf;

            if constexpr (not inc::is_same<decltype(nullptr), fmt_t>){
                length     += fmt.length();
                buf         = base_t::template align<item_t>(
                    length, xref the.length/*获取实际分配长度*/, inc::cast<inc::ialloc<item_t>>(alloc)
                );

                buf        += length;

                // fmt 剩下的内容排在最后
                buf        -= fmt.length();
                inc::copy_with_operator_unsafe(buf, fmt, fmt.length());
            }
            else{
                buf         = base_t::template align<item_t>(
                    length, xref the.length/*获取实际分配长度*/, inc::cast<inc::ialloc<item_t>>(alloc)
                );
                buf        += length;
            }

            // 缓冲区尾部地址，从后往前填充
            return buf;
        }
    protected:
        inc::ialloc<void> alloc;
        uxx               length;
    };
}

namespace mixc::lang_cxx_ph::origin::ph{
    template<class a0_t, class ... args_t>
    struct v : mixc::lang_cxx_ph::v<v<a0_t, args_t...>, a0_t, args_t...>{
        using base_t = mixc::lang_cxx_ph::v<v<a0_t, args_t...>, a0_t, args_t...>;

        v(a0_t const & first, args_t const & ... rest) : 
            base_t(first, rest...){}

        template<class item_t>
        inc::cxx<item_t> format(inc::cxx<item_t> fmt, inc::ialloc<item_t> const & alloc){
            inc::copy(xref base_t::alloc, alloc);
            item_t * ptr    = fmt.length() == 0 ? 
                base_t::template combine<item_t>(0) :
                base_t::template combine<item_t>(0, fmt);
            ptr            -= base_t::offset_to_head;
            return { ptr, base_t::length };
        }

        template<class item_t>
        inc::cxx<item_t> format(inc::ialloc<item_t> const & alloc){
            inc::copy(xref base_t::alloc, alloc);
            item_t * ptr    = base_t::template combine<item_t>(0);
            ptr            -= base_t::offset_to_head;
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