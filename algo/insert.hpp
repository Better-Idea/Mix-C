#ifndef xpack_algo_insert
#define xpack_algo_insert
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_insert
        #include"define/base_type.hpp"
        #include"interface/initializer_list.hpp"
        #include"interface/ranger.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"macro/xdebug_fail.hpp"
        #include"memop/copy.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_insert{
        template<class item_t>
        inline uxx insert(inc::ranger<item_t> target, ixx index, inc::ranger<item_t> values){
            xindex_rollback(target.length(), index, +1);
            xdebug_fail(index > target.length());
            auto len = target.length() + values.length();
            auto i   = target.length();
            auto j   = values.length();

            while(ixx(i) >= ixx(index)){
                i -= 1;
                target[i + j] = target[i];
            }
            for(i = 0; i < j; i++){
                target[i + index] = values[i];
            }
            return len;
        }

        template<class item_t>
        inline uxx insert(inc::ranger<item_t> target, ixx index, item_t const & value){
            return insert<item_t>(target, index, inc::initializer_list<item_t>{ value });
        }

        template<class item_t>
        inline uxx insert(
            inc::ranger<item_t> target, 
            inc::ranger<item_t> source, 
            ixx                 index, 
            inc::ranger<item_t> values){

            xindex_rollback(target.length(), index, +1);
            xdebug_fail(index > target.length());
            auto len = source.length() + values.length();
            auto a   = target.range(co{index});
            inc::copy_with_operator(target, source, index);
            inc::copy_with_operator(a, values, values.length());
            inc::copy_with_operator(a.range(co{values.length()}), source.range(co{index}), source.length() - index);
            return len;
        }

        template<class item_t>
        inline uxx insert(
            inc::ranger<item_t> target, 
            inc::ranger<item_t> source, 
            ixx                 index, 
            item_t const &      values){
            return insert<item_t>(target, source,index, inc::initializer_list<item_t>{ values });
        }
    }
#endif

namespace xuser::inc{
    using ::mixc::algo_insert::insert;
}
