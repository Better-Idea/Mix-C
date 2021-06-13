#ifndef xpack_lang_cxx_private_uri
#define xpack_lang_cxx_private_uri
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::lang_cxx_private_uri::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xref.hpp"
#include"utils/bits_bind.hpp"
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_private_uri::origin{
    struct uri_t{
    private:
        enum{
            bwidth = sizeof(uxx) * 8,
        };

        uxx items[128 / bwidth];
    public:
        constexpr uri_t():
            items{}{

            for(auto & item : items){
                item            = uxx(-1);
            }
        }

        inc::bits_bind<uxx> operator[](uxx i) const {
            return inc::bits_bind<uxx>{ xref(items[i / bwidth]), i % bwidth };
        }

        friend constexpr uri_t make_uri_lut();
    };

    constexpr u08 uri_lut[] = { 33,39,40,41,42,45,46,48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,95,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,126 };

    constexpr uri_t make_uri_lut(){
        uri_t result{};

        for(auto i : uri_lut){
            result.items[i / uri_t::bwidth]
                           ^= uxx(1) << (i % uri_t::bwidth);
        }
        return result;
    }
}

namespace mixc::lang_cxx_private_uri::lookup{
    constexpr origin::uri_t uri = origin::make_uri_lut();
}

#endif

xexport_spacex(lookup, mixc::lang_cxx_private_uri::lookup)
xexport_space (mixc::lang_cxx_private_uri::origin)
