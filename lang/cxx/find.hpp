#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_find
#define xpack_lang_cxx_find
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_find::inc
#include"interface/can_compare.hpp"
#include"lang/cxx/private/find.hpp"
#include"lang/cxx/private/find_mix.hpp"
#include"macro/xitr_foreach.hpp"
#include"mixc.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_find{
    #define xa_name             find
    #define xa_find_mix_type    0/*从前往后找*/
    #include"lang/cxx/private/xgen.find.hpp"
}

#endif

namespace mixc::lang_cxx_find::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_find::xuser
