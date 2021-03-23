#define xuser mixc::powerful_cat
#include"algo/insert.hpp"
#include"docker/array.hpp"
#include"mixc.hpp"
#include"utils/counter.hpp"

int run(){
    using namespace xuser;
    counter demo = 0;

    // 演示：序列中插入一个元素
    {
        xhint(demo);

        // 不设置初始元素的数组
        array<i32, 10>  buf;

        // 创建长度为 10 的数组，默认只设置开始的 4 个元素
        array<i32, 10>  ary{ 10, 20, 30, 40 };

        uxx             new_length;

        // 子序列引用
        // 该操作不会分配内存
        // 绑定 ary 数组 [0, 4) 区间内的 4 个元素
        auto    subary_ref  = ary.subseq(co{0, 4});

        // 将 15 插入到数组第二个元素 20 的前面（插入到索引 1）
        // 插入后数组一共有 5 个元素，即 new_length = 5
        new_length          = insert(subary_ref, 1, 15);

        // 新的长度为 5
        xhint(new_length);

        // 依次打印 10,15,20,30,40
        for(uxx i = 0; i < new_length; i++){
            xhint(i, ary[i]);
        }

        // 扩容
        // 由于 subary_ref 和 ary 使用的同一片内存
        // 所以之前的 insert 操作也会使得 ary 更改
        subary_ref          = ary.subseq(co{0, new_length});

        // 让源数组 subary_ref 的值保持不变
        // 将插入的结果写入目标数组 buf
        // 插入的索引支持负数，-1 表示倒数第一个元素的后边， -2 表示倒数第二个元素的后边，以此类推
        new_length          = insert(buf, subary_ref, -1/*index*/, 50/*value*/);

        // 新的长度为 6
        xhint(new_length);

        // 依次打印 10,15,20,30,40,50
        for(uxx i = 0; i < new_length; i++){
            xhint(i, buf[i]);
        }
    }

    // 演示：序列插入多个元素
    {
        xhint(demo);

        // 不设置初始元素的数组
        array<i32, 10>  buf;

        // 创建长度为 10 的数组，默认只设置开始的 4 个元素
        array<i32, 10>  ary{ 10, 20, 30, 40 };

        // 用于追加的数组
        auto &&         append_ary = array_view{ 50, 60, 70 };

        uxx             new_length;

        // 子序列引用
        // 该操作不会分配内存
        // 绑定 ary 数组 [0, 4) 区间内的 4 个元素
        auto    subary_ref  = ary.subseq(co{0, 4});

        // 将 append_ary 插入到数组末尾(索引为 -1)
        // 插入的索引支持负数，-1 表示倒数第一个元素的后边， -2 表示倒数第二个元素的后边，以此类推
        // 插入后数组一共有 7 个元素，即 new_length = 7
        new_length          = insert(subary_ref, -1, append_ary);

        // 新的长度为 7
        xhint(new_length);

        // 依次打印 10,20,30,40,50,60,70
        for(uxx i = 0; i < new_length; i++){
            xhint(i, ary[i]);
        }

        // 扩容
        // 由于 subary_ref 和 ary 使用的同一片内存
        // 所以之前的 insert 操作也会使得 ary 更改
        subary_ref          = ary.subseq(co{0, new_length});

        // 让源数组 subary_ref 的值保持不变
        // 将插入的结果写入目标数组 buf
        new_length          = insert(buf, subary_ref, 0/*index*/, array_view{ 1, 2, 3 }/*value*/);

        // 新的长度为 10
        xhint(new_length);

        // 依次打印 1,2,3,10,20,30,40,50,60,70
        for(uxx i = 0; i < new_length; i++){
            xhint(i, buf[i]);
        }
    }
    return 0;
}