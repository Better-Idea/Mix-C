#ifndef xpack_algo_heap_root
#define xpack_algo_heap_root
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_heap_root
        #include"define/base_type.hpp"
        #include"docker/transmitter.hpp"
        #include"dumb/mirror.hpp"
        #include"interface/can_compare.hpp"
        #include"interface/ranger.hpp"
        #include"memop/addressof.hpp"
        #include"memop/copy.hpp"
        #include"memory/new.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_heap_root{
        template<class item_t>
        struct heap_root{
            static void push(
                inc::ranger<item_t> const & range,
                uxx                         length,
                item_t              const & value, 
                inc::can_compare<item_t>    compare = inc::default_compare<item_t>){

                // 注意：
                // [value] 可能存在 [range] 中, 
                // 所以需要一个 [value] 的副本
                inc::mirror<item_t> similarities = value;
                uxx                 i            = length;
                uxx                 ii;
                
                for (; i > 0; i = ii){
                    auto & parent = range[
                        ii = (i - 1) >> 1
                    ];

                    if (compare(parent, similarities) <= 0){
                        break;
                    }

                    // 避免拷贝赋值
                    inc::copy(xref range[i], parent);
                }

                // 这里需要拷贝构造
                new (xref range[i]) item_t(similarities);
            }

            static auto pop(
                inc::ranger<item_t> const & range,
                uxx                         length,
                item_t              const & insert_value, 
                inc::can_compare<item_t>    compare = inc::default_compare<item_t>){
                
                // 避免返回值的复制构造
                auto && wanted     = inc::transmitter{ range[0] };
                uxx     i          = 0;
                uxx     left_index = 1;

                while(left_index + 1 < length) {
                    item_t & left  = range[left_index];
                    item_t & right = range[left_index + 1];
                    item_t * select;

                    // 小根堆 父节点要小于左右节点 所以要找子节点中较小者
                    if (compare(left, right) > 0){
                        select = xref right;
                        left_index++;
                    }
                    else{
                        select = xref left;
                    }

                    // 较小的子节点大于等于插入 就不用再下移了
                    if (compare(select[0], insert_value) >= 0){
                        break;
                    }

                    inc::copy(xref range[i], select[0]);
                    i          = (left_index);
                    left_index = (left_index << 1) + 1;
                }

                new (xref range[i]) item_t(insert_value);
                return wanted;
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::algo_heap_root::heap_root;
}

