/* 模块：hash
 * 类型：函数单体
 * 功能：得到一个对象的 hash 值
 * 用法：
 * TODO ===========================================================
 * 
 * 注意：
 * - 不同字节数的类型即使值相同也会具有不同的 hash 值
 * - hash(u08('a')) != hash(u16('a'))
 */

#ifndef xpack_algo_hash
#define xpack_algo_hash
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::algo_hash::inc
#include"instruction/ring_shift_left.hpp"
#include"instruction/count_of_set.hpp"
#include"interface/seqptr.hpp"
#include"memop/addressof.hpp"
#include"meta/is_origin_array.hpp"
#include"meta/is_same.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::algo_hash::origin{
    /* 函数：底层哈希函数
     * 参数：
     * - mem 为要 hash 的对象首地址
     * - blocks 为该对象可分成多少个 uxx 机器字长的块(向下取整)
     * - rest 为该对象向下取整时剩余的字节数
     * - seed 为随机数种子，用于避免 hash 攻击
     * 返回：
     * - hash 结果
     */
    inline uxx hash(voidp mem, uxx blocks, uxx rest, uxx seed = 0){
        uxxp ptr  = (uxxp)mem;
        uxx  mask = (uxx(1) << (rest * 8)) - 1;
        uxx  val  = (ptr[blocks] & mask);
        uxx  x    = (val + seed + rest);
        uxx  y    = (magic_number * (u32(-1) >> 1));

        for(uxx i = 0; i <= blocks; i++){
            y    += i == blocks ? ptr[i] & mask : ptr[i];
            x    += inc::count_of_set(y);
            y    += inc::ring_shift_left(x, y);
            x    += inc::ring_shift_left(y, x);
        }
        return x;
    }

    /* 函数：上层哈希函数
     * 参数：
     * - value 为要 hash 的对象
     * - seed 为随机数种子，用于避免 hash 攻击
     * 返回：
     * - hash 结果
     */
    template<class type>
    inline uxx hash(type const & value, uxx seed = 0){
        if constexpr (
            inc::is_same<type, asciis> or 
            inc::is_same<type, words> or 
            inc::is_origin_array<type>){

            uxx i = 0;
            while(value[i] != '\0'){
                i++;
            }
            return hash(voidp(value), i / sizeof(uxx), i % sizeof(uxx), seed);
        }
        else{
            return hash(inc::addressof(value), sizeof(type) / sizeof(uxx), sizeof(type) % sizeof(uxx), seed);
        }
    }
}
#endif

xexport_space(mixc::algo_hash::origin)
