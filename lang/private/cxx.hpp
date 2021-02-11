#ifndef xpack_lang_cxx
#define xpack_lang_cxx
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lang_cxx::inc
#include"algo/hash.hpp"
#include"define/base_type.hpp"
#include"interface/ranger.hpp"
#include"macro/xstruct.hpp"
#include"macro/xsv.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_hash::origin{
    template<class cxx_t>
    requires(sizeof(typename cxx_t::im_cxx) != 0)
    inline uxx hash(cxx_t const & value, uxx seed = 0){
        return hash(
            static_cast<typename cxx_t::item_t *>(value), 
            value.length() / sizeof(uxx), 
            value.length() % sizeof(uxx),
            seed
        );
    }
}

namespace mixc::lang_cxx{
    template<class item_t>
    static inline item_t empty = item_t(0);

    template<class final_t, class item_type>
    xstruct(
        xtmpl(cxx_t, final_t, item_type),
        xprif(ptr, item_type *),
        xprif(plength, uxx)
    )
        using im_cxx = final_t;
        using item_t = item_type;
    public:
        xranger(item_t)

        cxx_t() : cxx_t(& empty<item_t>, 0) {}
        cxx_t(cxx_t const &)        = default;

        cxx_t(item_t const * str) : 
            ptr((item_t *)str), plength(0) {

            if (ptr == nullptr){
                ptr                 = & empty<item_t>;
            }

            while(ptr[plength] != '\0'){
                plength            += 1;
            }
        }

        template<class finalx_t>
        cxx_t(cxx_t<finalx_t, item_t> const & self) : 
            cxx_t((the_t &)self){
        }

        template<class type>
        cxx_t(type const * ptr, uxx length) : 
            ptr((item_t *)ptr), plength(length) {
            static_assert(sizeof(type) == sizeof(item_t));
        }

        cxx_t(inc::static_string_holder<item_t> holder) : 
            cxx_t(holder.ptr(), holder.length()){
        }

        item_t & operator [](uxx index) const {
            return the.ptr[index];
        }

        template<class number_t>
        item_t & operator [](number_t const & index) const {
            return the.ptr[(uxx)(number_t &)index];
        }

        explicit operator item_t *() const {
            // 使用 explicit 避免出现在未包含 "lang/cxx/compare_xxx.hpp" 时直接变成指针比较 
            return the.ptr;
        }

        final_t backward(uxx value) const {
            auto tmp     = thex;
            tmp.ptr     += value;
            tmp.plength -= uxx(value);
            return tmp;
        }

        final_t forward(uxx value) const {
            return backward(
                uxx(-ixx(value))
            );
        }

        final_t shorten(uxx length) const {
            auto tmp     = thex;
            tmp.plength -= length;
            return tmp;
        }

        final_t elongate(uxx length) const {
            return shorten(
                uxx(-ixx(length))
            );
        }

    public:
        xpubget_pubset(length)

        xpubgetx(is_empty, bool){
            return length() == 0;
        }
    $

    template<class final_t, class item_t> struct cxx;
    template<class final_t>
    xstruct(
        xspec(cxx, final_t, char),
        xpubb(cxx_t<final_t, char>)
    )
        using base_t = cxx_t<final_t, char>;
        using base_t::base_t;

        xpubgetx(length_for_char16, uxx){
            auto c = uxx(0);
            auto p = the.ptr;
            auto e = the.ptr + the.length();

            for(; p < e; c++){
                while((++p)[-1] >= 0x80 and p < e){
                    ;
                }
            }
            return c;
        }
    };

    template<class final_t>
    xstruct(
        xspec(cxx, final_t, char16_t),
        xpubb(cxx_t<final_t, char16_t>)
    )
        using base_t = cxx_t<final_t, char16_t>;
        using base_t::base_t;

        xpubgetx(length_for_char, uxx){
            auto c = uxx(0);
            auto p = the.ptr;
            auto e = the.ptr + the.length();

            for(; p < e; p++){
                if (p[0] >= 0x800){
                    c += 3;
                }
                else if (p[0] >= 0x80){
                    c += 2;
                }
                else{
                    c += 1;
                }
            }
            return c;
        }
    };
}

#if xis_os64
    #define xc08(str)      xsv(char, "0123456789abcdef", str)
#elif xis_os32
    #define xc08(str)      xsv(char, "01234567", str)
#endif
#endif

#define xusing_lang_cxx     ::mixc::lang_cxx
