/*
概率分布 API —— probability 
样例：

#define xuser some_pack
#include<stdio.h>
#include"docker/array.hpp"
#include"math/distribution/probability.hpp"

namespace some_pack{
    void test(){
        // 指定索引出现的比例
        inc::probability<> distribution(
            1, 2, 3
        );

        // 计算指定索引出现的频数
        inc::array<uxx, 3> counts {};

        for (uxx i = 0; i < 600; i++){
            auto r = distribution.random();
            counts[r]++;
        }

        for (uxx i = 0; i < counts.length(); i++){
            printf("%u\n", (u32)counts[i]);
        }
    }
}

int main(){
    some_pack::test();
    return 0;
}

*/

#ifndef xpack_math_distribution_probability
#define xpack_math_distribution_probability
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::math_distribution_probability
        #include"algo/binary_search.hpp"
        #include"define/base_type.hpp"
        #include"docker/darray.hpp"
        #include"docker/array.hpp"
        #include"macro/xrange.hpp"
        #include"math/random.hpp"
        #include"meta/is_same.hpp"
        #include"meta/unsigned_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::math_distribution_probability{
        template<class type = u32>
        struct probability{
        private:
            using item_t = typename inc::unsigned_type<type>;
            inc::darray<item_t> guide;
        public:
            probability() = default;

            template<class ... args>
            probability(type first, type second, args const & ... list) : 
                probability(inc::array_view { first, second, list... }){
            }

            probability(inc::ranger<type> proportion) : 
                guide(
                    inc::length(proportion.length())
                ){

                item_t multi;

                // 约定当 proportion 是浮点序列时，我们将其中的元素放大 multi 倍后整数化
                // 当 proportion 为整数序列则无需乘以 multi（让 multi 等于 1）
                if constexpr (inc::is_same<type, f32>){ 
                    multi = 1024 * 1024 * 1024;
                }
                else if constexpr (inc::is_same<type, f64>){
                    multi = 1024ull * 1024 * 1024 * 1024 * 1024;
                }
                else{
                    multi = 1;
                }

                guide[0] = item_t(proportion[0] * multi) - 1;

                // proportion 中存放的是概率比序列
                //    - 比如：存的是 { 2, 3, 4 }，指示的是
                //      0 号元素出现的概率为 2/9，
                //      1 号元素出现的概率为 3/9，
                //      1 号元素出现的概率为 4/9
                // guide 中存放的是由 proportion 转换而来的范围序列，加入 proportion 依旧存放的是 { 2, 3, 4 }
                //    - 那么：guide 将存放 { 1, 4, 8 }
                //      gudie 中两个彼此相邻的元素构成一个范围区间，下标为 0 的元素以 0 为左闭区间
                //      由此可以知道随机生成的数字（值不大于区间左右边界）落在区间 { [0, 1], [2, 4], [5, 8] } 对应概率为 { 2，3，4 } 
                //    - 通过 inc::random 生成范围不大于 guide 中最大元素的随机值 r，
                //      然后用 match_down 二分搜索，在 guide 中寻找不大于 r 但最接近 r 值的索引
                for (uxx i = 1, length = proportion.length(); i < length; i++){
                    guide[i] = guide[i - 1] + item_t(proportion[i] * multi);
                }
            }

            uxx random() const {
                auto l = guide.length();
                auto r = inc::random<item_t>() % (guide[l - 1] + 1);
                auto v = inc::binary_search<item_t>::match_down(guide, r);
                return v;
            }

            template<class item> item & random(inc::ranger<item> items) const {
                return items[random()];
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::math_distribution_probability::probability;
}
