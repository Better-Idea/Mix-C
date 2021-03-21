#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_find_reserve
#define xpack_lang_cxx_find_reserve
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_find_reserve::inc
#include"interface/can_compare.hpp"
#include"interface/initializer_list.hpp"
#include"interface/unified_seq.hpp"
#include"lang/cxx/private/find_mix.hpp"
#include"lang/cxx/private/find_reserve.hpp"
#include"macro/xitr_foreach.hpp"
#include"meta/has_cast.hpp"
#include"meta/item_origin_of.hpp"
#include"mixc.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_find_reserve{
    #define xa_name             find_reserve
    #define xa_find_mix_type    1/*从后往前找*/
    #include"lang/cxx/private/xgen.find.hpp"
}

#endif

namespace mixc::lang_cxx_find_reserve::xuser {
    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_find_reserve::xuser
