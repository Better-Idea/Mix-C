#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_format
#define xpack_lang_cxx_format
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_format
        #include"define/base_type.hpp"
        #include"interface/can_alloc.hpp"
        #include"lang/cxx/strlize.hpp"
        #include"lang/cxx.hpp"
        #include"memop/base_seqlize.hpp"
        #include"memop/copy.hpp"
        #include"memop/fill.hpp"
        #include"meta/unsigned_type.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_format{
        /*
        * h{}      hex
        * H{}      upper hex 
        * zh{}     hex with leading zero
        * zH{}     upper hex with leading zero
        * x{}      hex with 0x prefix
        * X{}      upper hex with 0x prefix
        * zx{}     hex with 0x prefix and leading zero
        * zX{}     upper hex with 0x prefix and leading zero
        * 
        * v{}      normal value
        * d{}      dec(pending)
        * 
        * b{}      bin
        * zb{}     bin with leading zero
        * 
        * .l(5)    align left
        * .r(5)    align right
        * .c(5)    align center
        * 
        * more... to be continue
        */

        template<class final, class type>
        struct base{
        protected:
            type        value;
            uxx         align_mode          : 2  = 0;
            uxx         numeration          : 6  = 10;
            uxx         align_width              = 0;

            enum{
                align_center,
                align_left,
                align_right,
            };
        public:
            base(type const & value) : 
                value(value){}

            final & c(uxx align_center_width){
                align_mode  = align_center;
                align_width = align_center_width;
                return thex;
            }

            final & l(uxx align_left_width){
                align_mode  = align_left;
                align_width = align_left_width;
                return thex;
            }

            final & r(uxx align_right_width){
                align_mode  = align_right;
                align_width = align_right_width;
                return thex;
            }
        };

        template<class final, class type>
        struct basex : base<final, type>{
            using the_t = base<final, type>;
            using the_t::the_t;

            template<class item_t>
            item_t * align(uxx length, inc::can_alloc<item_t> alloc){
                if (the_t::align_width == 0 or the_t::align_width <= length){
                    return alloc(length);
                }

                auto mem       = alloc(the_t::align_width);
                auto pad_width = the_t::align_width - length;
                auto half      = pad_width / 2;

                switch(the_t::align_mode){
                case the_t::align_center:
                    inc::fill(mem, ' ', half);
                    inc::fill(length + (mem += half), ' ', pad_width - half);
                    return mem;
                case the_t::align_left:
                    inc::fill(mem + length, ' ', pad_width);
                    return mem;
                default: // align_right
                    inc::fill(mem, ' ', pad_width);
                    return mem + pad_width;
                }
            }
        };

        #define xbase   (*(basex<decltype(*this), type> *)this)
        #define xopt                                                                        \
            void operator >> (inc::can_alloc<char> alloc){ output<char>(alloc); }           \
            void operator >> (inc::can_alloc<w16> alloc){ output<w16>(alloc); }             \
        private:                                                                            \
            template<class item_t>                                                          \
            void output(inc::can_alloc<item_t> alloc)

        template<
            class             final, 
            class             type, 
            inc::numeration_t n,
            bool              with_prefix, 
            bool              keep_leading_zero, 
            auto              lut>
        struct hex : base<final, type>{
            using the_t = base<final, type>;
            using the_t::the_t;

            xopt{
                inc::cxx<item_t>(the_t::value, n, lut, [this, alloc](uxx length){
                    auto new_length = (keep_leading_zero ? sizeof(type) * 2 : length);
                    auto zero_count = new_length - length;
                    auto mem        = xbase.template align<item_t>(
                        new_length += (with_prefix ? 2 : 0), 
                        alloc
                    );

                    if (with_prefix){
                        inc::copy(mem, "0x", 2);
                        mem += 2;
                    }
                    if (zero_count){
                        inc::fill(mem, '0', zero_count);
                        mem += zero_count;
                    }
                    return mem;
                });
            }
        };

        constexpr bool with_prefix       = true;
        constexpr bool keep_leading_zero = true;

        #define xnum(name,type_t,numeration,prefix,leading_zero,lut)                            \
            template<class type>                                                                \
            struct name : hex<name<type>, type_t, numeration, prefix, leading_zero, lut> {      \
                name(type const & value) :                                                      \
                    hex<name<type>, type_t, numeration, prefix, leading_zero, lut>(value){}     \
            }
        
        #define xhex(name,prefix,leading_zero,lut)      \
            xnum(name,inc::unsigned_type<type>,inc::numeration::hex,prefix,leading_zero,lut)

        #define xbin(name,prefix,leading_zero,lut)      \
            xnum(name,inc::unsigned_type<type>,inc::numeration::bin,prefix,leading_zero,lut)

        #define xoct(name,prefix,leading_zero,lut)      \
            xnum(name,inc::unsigned_type<type>,inc::numeration::oct,prefix,leading_zero,lut)

        #define xstr(type,ptr) template<uxx n> struct v<type[n]> : v<ptr>{ using v<ptr>::v; }

    }

    namespace mixc::lang_cxx_format::ph{ // place_holder group
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

        template<class type>
        struct v : base<v<type>, type>{
            using the_t = base<v<type>, type>;
            v(type const & value)
                : the_t(value){}

            xopt {
                inc::cxx<item_t>(the_t::value, [this, alloc](uxx length){
                    return xbase.template align<item_t>(length, alloc);
                });
            }
        };

        xstr(char, asciis);
        xstr(w16, words);

        #undef  xstr
        #undef  xbin
        #undef  xoct
        #undef  xhex
        #undef  xnum
        #undef  xopt 
        #undef  xmate
    }

    namespace mixc::lang_cxx_format{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto format() const {

            }
        };
    }
#endif

namespace mixc::lang_cxx_format::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;
    };
}

namespace xuser::inc::ph{
    using namespace ::mixc::lang_cxx_format::ph;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_format::xuser
