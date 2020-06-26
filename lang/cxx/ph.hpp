#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_ph
#define xpack_lang_cxx_ph
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
    #undef  xuser
    #define xuser mixc::lang_cxx_ph
    #include"define/base_type.hpp"
    #include"interface/can_alloc.hpp"
    #include"lang/cxx/strlize.hpp"
    #include"lang/cxx.hpp"
    #include"macro/xdebug_fail.hpp"
    #include"memop/addressof.hpp"
    #include"memop/copy.hpp"
    #include"memop/fill.hpp"
    #include"meta/is_based_on.hpp"
    #include"meta/is_ptr.hpp"
    #include"meta/unsigned_type.hpp"

    namespace xuser{
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

        struct place_holder_group{};

        template<class final, class type>
        struct base_ph : place_holder_group{
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
            base_ph(){}
            base_ph(type const & value) : 
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
        struct basex_ph : base_ph<final, type>{
            using the_t = base_ph<final, type>;
            using the_t::the_t;
            basex_ph(){}

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
                    inc::fill_with_operator(mem, ' ', half);
                    inc::fill_with_operator(length + (mem += half), ' ', pad_width - half);
                    return mem;
                case the_t::align_left:
                    inc::fill_with_operator(mem + length, ' ', pad_width);
                    return mem;
                default: // align_right
                    inc::fill_with_operator(mem, ' ', pad_width);
                    return mem + pad_width;
                }
            }
        };

        #define xbase   (*(basex_ph<decltype(*this), type> *)this)
        #define xopt                                                                            \
            inc::c08 operator >> (inc::can_alloc<char> alloc) { return output(alloc); }         \
            inc::c16 operator >> (inc::can_alloc<char16_t>  alloc) { return output(alloc); }    \
            template<class item_t>                                                              \
            inc::cxx<item_t> output(inc::can_alloc<item_t> alloc)

        template<
            class             final, 
            class             type, 
            inc::numeration_t n,
            bool              with_prefix, 
            bool              keep_leading_zero, 
            auto              lut>
        struct num : base_ph<final, type>{
            using the_t = base_ph<final, type>;
            using the_t::the_t;
            num(){}

            xopt{
                auto deformation = [this](){
                    if constexpr (inc::is_ptr<type>){
                        return uxx(the_t::value);
                    }
                    else{
                        return the_t::value;
                    }
                };
                return inc::cxx<item_t>(deformation(), n,  lut, [this, alloc](uxx length){
                    auto klz_length = sizeof(type) * 8; // keep leading zero length

                    if constexpr (n == inc::numeration::hex){
                        klz_length /= 4;
                    }
                    else if constexpr (n == inc::numeration::oct){
                        klz_length = klz_length / 3 + (klz_length % 3 != 0);
                    }
                    else if constexpr (n == inc::numeration::bin){
                        klz_length = klz_length;
                    }

                    auto new_length = (keep_leading_zero ? klz_length : length);
                    auto zero_count = new_length - length;
                    auto mem        = xbase.template align<item_t>(
                        new_length += (with_prefix ? 2 : 0), 
                        alloc
                    );

                    if constexpr (with_prefix){ // only in hex
                        inc::copy_with_operator(mem, "0x", 2);
                        mem += 2;
                    }
                    if (zero_count){
                        inc::fill_with_operator(mem, '0', zero_count);
                        mem += zero_count;
                    }
                    return mem;
                });
            }
        };

        constexpr bool with_prefix       = true;
        constexpr bool keep_leading_zero = true;

        #define xnum(name,type_t,numeration,prefix,leading_zero,lut)                                    \
            template<class type>                                                                        \
            struct name : num<name<type>, type_t, numeration, prefix, leading_zero, lut> {              \
                name(){}                                                                                \
                name(type const & value) :                                                              \
                    num<name<type>, type_t, numeration, prefix, leading_zero, lut>(type_t(value)){}     \
            }
        
        #define xhex(name,prefix,leading_zero,lut)      \
            xnum(name,inc::unsigned_type<type>,inc::numeration::hex,prefix,leading_zero,lut)

        #define xbin(name,prefix,leading_zero,lut)      \
            xnum(name,inc::unsigned_type<type>,inc::numeration::bin,prefix,leading_zero,lut)

        #define xoct(name,prefix,leading_zero,lut)      \
            xnum(name,inc::unsigned_type<type>,inc::numeration::oct,prefix,leading_zero,lut)

        #define xstr(type,ptr) template<uxx n> struct v<type[n]> : v<ptr>{ using v<ptr>::v; }
    }

    namespace xuser::ph{
        using xuser::place_holder_group;
        template<class> struct v;
        template<class> struct zx;

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
        struct v : base_ph<v<type>, type>{
            using the_t = base_ph<v<type>, type>;
            v(){}
            v(type const & value)
                : the_t(value){}

            xopt {
                return inc::cxx<item_t>(the_t::value, [this, alloc](uxx length){
                    return xbase.template align<item_t>(length, alloc);
                });
            }
        };

        xstr(char, asciis);
        xstr(char16_t, words);
    }

    namespace xuser{
        template<class ... args> struct phg_core;
        template<class a0, class ... args>
        struct phg_core<a0, args...> : phg_core<args...>{
        private:
            static auto ph_type(){
                if constexpr (inc::is_based_on<place_holder_group, a0>){
                    return a0();
                }
                else if constexpr (inc::is_ptr<a0>){
                    return ph::zx<voidp>();
                }
                else{
                    return ph::v<a0>();
                }
            }

            decltype(ph_type()) item;
            using base_t = phg_core<args...>;
        public:
            phg_core(a0 const & first, args const & ... list) : 
                base_t(list...), item(first){
            }

            template<class item_t>
            inc::cxx<item_t> output(uxx old_length){
                inc::cxx<item_t> ret;
                item.template output<item_t>([&](uxx length) -> item_t * {
                    ret = base_t::template output<item_t>(old_length + length);
                    ret = ret.forward(length);
                    return ret;
                });
                return ret;
            }
        };

        template<> struct phg_core<>{
            inc::can_alloc<void> alloc;

            template<class item_t>
            inc::cxx<item_t> output(uxx length){
                return { (item_t *)alloc(length) + length, length };
            }
        };
    }

    namespace mixc::lang_cxx_ph::ph{ // place_holder
        template<class a0, class ... args>
        struct phg{ // place_holder group
            phg(a0 const & first, args const & ... list)
                : items(first, list...){
            }

            xopt {
                inc::copy(xref items.alloc, alloc);
                return items.template output<item_t>(0);
            }
        private:
            phg_core<a0, args...> items;
        };
    }

    #undef  xstr
    #undef  xbin
    #undef  xoct
    #undef  xhex
    #undef  xnum
    #undef  xopt 
    #undef  xmate
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace xuser::inc::ph{
    using namespace ::mixc::lang_cxx_ph::ph;
}

