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
    inline auto invoke(){
        using item_t     = inc::item_origin_of<seq_t>;
        using plan_a     = inc::seqptr<item_t>;
        using plan_b     = inc::ranger<item_t>;

        if constexpr (inc::has_cast<plan_a, seq_t>){
            return plan_a();
        }
        else{
            return plan_b();
        }
    }
}

namespace mixc::meta_unified_seq::origin{
    template<class seq_t>
    concept unified_seq_t = 
        inc::has_cast<inc::seqptr<inc::item_origin_of<seq_t>>, seq_t> or 
        inc::has_cast<inc::ranger<inc::item_origin_of<seq_t>>, seq_t>;

    template<unified_seq_t seq_t>
    using unified_seq = decltype(
        invoke<seq_t>()
    );
}

#endif

xexport_space(mixc::meta_unified_seq::origin)