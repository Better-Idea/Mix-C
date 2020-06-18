#ifndef xpack_algo_remove
#define xpack_algo_remove
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_remove
        #include"define/base_type.hpp"
        #include"interface/ranger.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"macro/xdebug_fail.hpp"
        #include"memop/swap.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_remove{
        template<class item_t>
        inline uxx remove(inc::ranger<item_t> const & target, iinterval range){
            range.normalize(target.length());
            auto l = range.left();
            auto r = range.right();
            xdebug_fail(l >= target.length());
            xdebug_fail(r >= target.length());

            if (l > r){
                inc::swap(& l, & r);
            }

            auto i   = 0;
            auto j   = target.length() - r - 1;
            auto len = j + l;

            while(i < j){
                target[l + i] = target[r + i + 1];
                i            += 1;
            }
            while(i < target.length()){
                target[l + i].~item_t();
                i += 1;
            }
            return len;
        }

        template<class item_t>
        inline uxx remove(inc::ranger<item_t> const & target, ixx index){
            xindex_rollback(target.length(), index);
            return remove<item_t>(target, cc{ index, index });
        }

        // template<class item_t>
        // inline uxx remove(
        //     inc::ranger<item_t> target, 
        //     inc::ranger<item_t> source, 
        //     ixx                 index, 
        //     inc::ranger<item_t> values){

        //     return len;
        // }

        // template<class item_t>
        // inline uxx remove(
        //     inc::ranger<item_t> target, 
        //     inc::ranger<item_t> source, 
        //     ixx                 index, 
        //     item_t const &      values){
        //     return remove<item_t>(target, source,index, inc::initializer_list<item_t>{ values });
        // }
    }
#endif

namespace xuser::inc{
    using ::mixc::algo_remove::remove;
}
