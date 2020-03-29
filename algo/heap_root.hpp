#ifndef xpack_algo_heap_root
#define xpack_algo_heap_root
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_heap_root
        #include"define/base_type.hpp"
        #include"docker/transmitter.hpp"
        #include"dumb/mirror.hpp"
        #include"macro/xref.hpp"
        #include"memop/copy.hpp"
        #include"memory/new.hpp"
        #include"interface/can_compare.hpp"
        #include"interface/can_random_access.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_heap_root{
        template<class item_t>
        struct heap_root{
            static void push(
                inc::can_random_access<item_t> sequence,
                uxx                            length,
                item_t const &                 value, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>){

                // 注意：
                // [value] 可能存在 [sequence] 中, 
                // 所以需要一个 [value] 的副本
                inc::mirror<item_t> similarities = value;
                uxx                 i            = length;
                uxx                 ii;
                
                for (; i > 0; i = ii){
                    auto & parent = sequence[
                        ii = (i - 1) >> 1
                    ];

                    if (compare(parent, similarities) <= 0){
                        break;
                    }

                    // 避免拷贝赋值
                    inc::copy(xref sequence[i], parent);
                }

                // 这里需要拷贝构造
                new (xref sequence[i]) item_t(similarities);
            }

            static auto pop(
                inc::can_random_access<item_t> sequence,
                uxx                            length,
                item_t const &                 insert_value, 
                inc::can_compare<item_t>       compare = inc::default_compare<item_t>){
                
                // 避免返回值的复制构造
                auto && wanted     = inc::transmitter{ sequence[0] };
                uxx     i          = 0;
                uxx     left_index = 1;
                
                while(left_index + 1 < length) {
                    item_t & left  = sequence[left_index];
                    item_t & right = sequence[left_index + 1];
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

                    inc::copy(xref sequence[i], select[0]);
                    i          = (left_index);
                    left_index = (left_index << 1) + 1;
                }

                new (xref sequence[i]) item_t(insert_value);
                return wanted;
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::algo_heap_root::heap_root;
}

