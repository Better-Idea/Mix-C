#ifndef xpack_meta_unified_seq
#define xpack_meta_unified_seq
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_unified_seq
#include"define/base_type.hpp"
#include"interface/ranger.hpp"
#include"interface/seqptr.hpp"
#include"meta/has_cast.hpp"
#include"meta/item_origin_of.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_unified_seq::origin{
    template<class seq_t>
    concept unified_seq_t = 
        inc::has_cast<inc::seqptr<inc::item_origin_of<seq_t>>, seq_t> or 
        inc::has_cast<inc::ranger<inc::item_origin_of<seq_t>>, seq_t>;

    template<unified_seq_t seq_t, class invoke_t>
    inline auto unified_seq_call(seq_t const & seq, invoke_t const & call){
        using item_t     = inc::item_origin_of<seq_t>;
        using plan_a     = inc::seqptr<item_t>;
        using plan_b     = inc::ranger<item_t>;

        if constexpr (inc::has_cast<plan_a, seq_t>){
            return call(plan_a(seq));
        }
        else{
            return call(plan_b(seq));
        }
    }
}

#endif

namespace xuser::inc{
    using namespace ::mixc::meta_unified_seq::origin;
}