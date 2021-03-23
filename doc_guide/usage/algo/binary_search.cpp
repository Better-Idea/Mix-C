#define xuser mixc::powerful_cat
#include"algo/binary_search.hpp"
#include"docker/array.hpp"
#include"mixc.hpp"
#include"utils/counter.hpp"

int run(){
    using namespace xuser;
    counter demo = 0;

    // 演示：在升序数组中寻找指定值对应的索引
    {
        xhint(demo);

        auto && ary     = array_view{ 0, 10, 20, 30, 40, 50, };
        uxx     index   = binary_search::match(ary, 50);

        // index 为 5，表示期望匹配的值 50 在 ary[5] 这个位置
        xhint(index);

        index           = binary_search::match(ary, 51);

        // 由于期望值 51 不在数组中，所以返回 not_exist
        xhint(index, not_exist);
    }

    // 演示：当目标序列是降序，如何使用二分匹配呢？
    {
        xhint(demo);

        auto && ary     = array_view{ 50, 40, 30, 20, 10, 0 };

        // 默认情况，binary_search 要求数组是升序的，降序的数组将得到错误的值
        uxx     index   = binary_search::match(ary, 50);

        xhint(index);

        // 此时我们给 binary_search::match 传入第三个参数 default_compare_neg<i32>
        // default_compare_neg 是个模板变量，实际上是个 lambda 变量，可以根据传入的类型 i32
        // 让 i32 类型的 ary 数组可以进行比较，不过它比较的顺序是倒过来的
        index           = binary_search::match(ary, 50, default_compare_neg<i32>);

        // 由于 ary 是降序的，而参与比较的回调也是反向比较的，所以此时就可以得到正确的值
        // index 为 0
        xhint(index);

        // 实际上也可以给手动 diy 一个 lambda
        // 这个 lambda 参数的作用就是参与二分查找的比较
        // 我们约定：
        // 在升序数组中参与比较的 lambda 返回正数时表示当前值大于期望值
        // 负数表示当前值小于期望值，零表示匹配期望值
        // 由于这里是降序的数组，所以逻辑是反过来的
        index           = binary_search::match(ary, 50, [](i32 const & current, i32 const & wanted) -> ixx/* 返回值类型为有符号整数类型 */ {
            // 由于 ary 是反序的，所以是 wanted - current，如果 ary 是正序的
            // lambda 函数体中的内容应该 current - wanted
            return ixx(wanted - current);
        });

        xhint(index);

        // 上面的 lambda 比较繁琐而且如果格式不匹配会导致编译错误
        // 所以我们推荐使用 xcmp 宏函数简化
        // left 是 xcmp 中定义的第一个参数，相当于上文的 current
        // right 是 xcmp 中定义的第二个参数，相当于上文的 wanted
        index           = binary_search::match(ary, 50, []xcmp(i32){
            return ixx(right - left);
        });

        // 实现的效果是一样的
        xhint(index);
    }

    // 演示：当目标值不存在时，退而求其次
    {
        xhint(demo);

        auto && ary     = array_view{ 0, 10, 20, 30, 40, 50, };
        uxx     index   = binary_search::less_equals(ary, 51);

        // index 为 5，因为不存在元素 51，所以只能找最接近但不超过 51 的值
        // 合适的值为 50，对应的索引为 5
        xhint(index);

        // 假如目标值存在，那么它的行为和 binary_search::match 是一致的
        index           = binary_search::less_equals(ary, 50);

        xhint(index);

        // 假如数组中没有比目标值更小的元素且不存在相等的值，那么它将返回 not_exist
        index           = binary_search::less_equals(ary, -2);

        xhint(index, not_exist);

        // binary_search::greater_equals 是类似的，它是找大于等于期望值的元素
        index           = binary_search::greater_equals(ary, 11);

        // 目标值 >= 11，匹配的值为 20，index 为 2
        xhint(index);

        // 类似的，如果数组中没有比目标值更大的元素且不存在相等的值，那么它将返回 not_exist
        index           = binary_search::greater_equals(ary, 51);

        xhint(index, not_exist);
    }

    // 演示：binary_search 提供更高级别的抽象
    {
        xhint(demo);

        auto && ary     = array_view{ 0, 10, 20, 30, 40, 50, };

        // 传入序列的长度和比较回调
        // 参与比较的可以是任何你期望的元素，binary_search 负责进行二分，你负责编写比较回调
        uxx     index   = binary_search::match(ary.length(), [&/* 这里使用引用捕获外部 ary */](uxx i_current) -> ixx {
            return ary[i_current] - 50; // 期望值为 50
        });

        xhint(index);

        // binary_search::less_equals 和 binary_search::greater_equals 也具有类似的操作
        // 这里就不赘述
    }
    return 0;
}
