#ifndef xpack_meta_seq_tpush_front
#define xpack_meta_seq_tpush_front
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_seq_tpush_front::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_seq_tpush_front{
    using namespace inc;

    template<class tlist, class item> struct tpush_front;
    template<class item, class ... args>
    struct tpush_front<tlist<args...>, item>{
        using new_list = tlist<item, args...>;
    };
}

#endif

xexport(mixc::meta_seq_tpush_front::tpush_front)