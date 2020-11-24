/*
unified_seq 
- 提供 seqptr<item_t>、ranger<item_t> 两种标准
- 优先使用 seqptr<item_t> 以实现完整性能
- 当需要使用反序功能时才会选择 ranger<item_t>
- seqptr<item_t> 仅支持正序范围的切片访问
*/
#ifndef xpack_meta_unified_seq
#define xpack_meta_unified_seq
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_unified_seq::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"interface/ranger.hpp"
#include"interface/seqptr.hpp"
#include"meta/has_cast.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_unified_seq{
    template<class seq_t>
    inline auto meta(){
        using item_t = inc::item_origin_of<seq_t>;

        if constexpr (inc::can_seqptrlize<seq_t>){
            return inc::seqptr<item_t>{};
        }
        else{
            return inc::ranger<item_t>{};
        }
    }
}

namespace mixc::meta_unified_seq::origin{
    template<class seq_t>
    concept can_unified_seqlize = inc::can_seqptrlize<seq_t> or inc::can_rangerlize<seq_t>;

    template<class seq_t, class item_t>
    concept can_unified_seqlizex = inc::can_seqptrlizex<seq_t, item_t> or inc::can_rangerlizex<seq_t, item_t>;

    template<can_unified_seqlize seq_t>
    using unified_seq = decltype(
        meta<seq_t>()
    );
}

#endif

xexport_space(mixc::meta_unified_seq::origin)