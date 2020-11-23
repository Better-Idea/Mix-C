/* 模块：bit_indicator
 * 类型：基础结构
 * 功能：提供静态/动态的位图结构
 * 用法：
 * TODO======================================================================================
 */

#ifndef xpack_docker_bit_indicator
#define xpack_docker_bit_indicator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_bit_indicator::inc
#include"docker/private/adapter.bit_indicator.hpp"
#include"dumb/disable_copy.hpp"
#include"interface/can_alloc.hpp"
#include"lock/atom_swap.hpp"
#include"memop/zeros.hpp"
#include"memory/allocator.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_bit_indicator{
    /* 结构：静态位图
     * 参数：
     * - final 为接口结构类型
     * - bits 为总位数
     */
    template<class final, uxx bits = 0>
    xstruct(
        xtmpl(bit_indicator_t, final, bits)
    )
    private:
        static constexpr uxx lv0 = bits / inc::bwidth + (bits % inc::bwidth != 0);
        static constexpr uxx lv1 = lv0 <= 1 ? 0 : lv0 / inc::bwidth + (lv0 % inc::bwidth != 0);
        static constexpr uxx lv2 = lv1 <= 1 ? 0 : lv1 / inc::bwidth + (lv1 % inc::bwidth != 0);
        static constexpr uxx lv3 = lv2 <= 1 ? 0 : lv2 / inc::bwidth + (lv2 % inc::bwidth != 0);
        static constexpr uxx lv4 = lv3 <= 1 ? 0 : lv3 / inc::bwidth + (lv3 % inc::bwidth != 0);
        static constexpr uxx lv5 = lv4 <= 1 ? 0 : lv4 / inc::bwidth + (lv4 % inc::bwidth != 0);
        static constexpr uxx lv6 = lv5 <= 1 ? 0 : lv5 / inc::bwidth + (lv5 % inc::bwidth != 0);
        static constexpr uxx lv7 = lv6 <= 1 ? 0 : lv6 / inc::bwidth + (lv6 % inc::bwidth != 0);
        static constexpr uxx lv8 = lv7 <= 1 ? 0 : lv7 / inc::bwidth + (lv7 % inc::bwidth != 0);
        static constexpr uxx lv9 = lv8 <= 1 ? 0 : lv8 / inc::bwidth + (lv8 % inc::bwidth != 0);
        static constexpr uxx lut[] = { lv0,lv1,lv2,lv3,lv4,lv5,lv6,lv7,lv8,lv9 };
    protected:
        /* 属性：位图树每一级的元素个数
         * 补充：lut[0] 表示第一级位图字的个数，对于 64bit 系统一个字有 64bit
         *       位图是分层的，在 64bit 系统，上层用 1bit 表示下层 64bit 位组中是否有置位位
         *       该属性的每一个元素分别指示了每次层一共有多少个字
         */
        static const uxx * level_lut(){
            return lut;
        }

        /* 属性：位图树中一共包含的字数 */
        static constexpr uxx size(){
            return lv0 + lv1 + lv2 + lv3 + lv4 + lv5 + lv6 + lv7 + lv8 + lv9;
        }

        /* 属性：位图树的高度 */
        static constexpr uxx height(){
            return 
                lv9 ? 10 : 
                lv8 ? 9  : 
                lv7 ? 8  : 
                lv6 ? 7  : 
                lv5 ? 6  : 
                lv4 ? 5  : 
                lv3 ? 4  : 
                lv2 ? 3  : 
                lv1 ? 2  : 1;
        }

        /* 属性：位图的首地址 */
        uxx * bmp() const {
            return data;
        }

        mutable uxx data[size()] = {0};

        /* 位图可访问的总位数 */
        xpubgetx(total_bits, uxx){
            return bits;
        }
    $

    /* 结构：动态位图
     * 参数：
     * - final 为接口结构类型
     */
    template<class final>
    xstruct(
        xspec(bit_indicator_t, final),
        xpubb(inc::disable_copy),
        xprif(pbmp          , mutable uxx *),
        xprif(pheight       , mutable uxx),
        xprif(psize         , mutable uxx),
        xprif(ptotal_bits   , mutable uxx)
    )
        bit_indicator_t() : 
            pbmp(nullptr), pheight(0), psize(0) {}

        /* 函数：构造函数（外部分配内存）
         * 参数：
         * - total_bits 为位图树的总位数
         * - alloc 为分配回调函数，期望签名如下
         *      uxx * alloc(uxx length);
         * 注意：
         * - 该方式需要外部释放内存
         */
        bit_indicator_t(uxx total_bits, inc::ialloc<uxx> alloc){
            uxx buf[sizeof(uxx) * 8 / inc::bwidth + 1];

            // 先设置，之后 total_bits 会被修改
            the.total_bits(total_bits);
            the.pheight = 0;
            the.psize   = 0;

            do{
                total_bits         = total_bits / inc::bwidth + (total_bits % inc::bwidth != 0);
                buf[the.pheight]   = total_bits;
                the.pheight       += 1;
                the.psize         += total_bits;
            }while(total_bits > 1);

            the.size(psize);
            the.bmp(
                alloc(cost_count())
            );

            for(uxx i = 0; i < the.height(); i++){
                the.level_lut()[i] = buf[i];
            }

            inc::zeros(the.bmp(), the.size() * sizeof(uxx));
            the.need_free(false);
        }

        /* 函数：构造函数（内部分配内存）
         * 参数：
         * - total_bits 为位图树的总位数
         * 注意：
         * - 该方式由内部释放内存
         */
        bit_indicator_t(uxx total_bits) : 
            bit_indicator_t(total_bits, [](uxx length){
                return inc::alloc<uxx>(
                    inc::memory_size{
                        sizeof(uxx) * length
                    }
                );
            }){
            the.need_free(true);
        }

    protected:
        ~bit_indicator_t(){
            if (not the.need_free()){
                return;
            }

            auto bytes = cost_bytes();
            auto ptr   = inc::atom_swap<uxx *>(& pbmp, nullptr);

            if (ptr != nullptr){
                inc::free(ptr - the.height(), bytes);
            }
        }

        /* 属性：位图树的高度 */
        xproget(height)
        xproget_prisetx(size, uxx)
            xr{ return the.psize >> 1; }
            xw{ 
                the.psize &= 1;
                the.psize |= value << 1;
            }

        /* 属性：位图树存放位图的首地址 */
        xproget_prisetx(bmp, uxx *)
            xr{ return the.pbmp; }
            xw{ the.pbmp = value + the.height(); }

        /* 属性：是否需要在析构时释放内存 */
        xpriget_prisetx(need_free, bool)
            xr{ return the.psize & 1; }
            xw{ the.psize = value ? the.psize | 1 : the.psize & (uxx(-1) << 1); }

        /* 属性：位图树每一级的元素个数
         * 补充：lut[0] 表示第一级位图字的个数，对于 64bit 系统一个字有 64bit
         *       位图是分层的，在 64bit 系统，上层用 1bit 表示下层 64bit 位组中是否有置位位
         *       该属性的每一个元素分别指示了每次层一共有多少个字
         */
        xprogetx(level_lut, uxx *){
            return the.pbmp - the.height();
        }

        /* 属性：动态内存部分一共有多少个字 */
        xprigetx(cost_count, uxx){
            return the.size() + the.height();
        }

        /* 属性：动态内存部分一共包含多少个字节 */
        xpubgetx(cost_bytes, inc::memory_size){
            return inc::memory_size(cost_count() * sizeof(uxx));
        }

        /* 属性：位图可访问的总位数 */
        xpubget_priset(total_bits);
    $

    template<class final, uxx total_bits>
    using bit_indicator = inc::adapter_bit_indicator<
        bit_indicator_t<final, total_bits>
    >;
}
#endif

#define xusing_docker_bit_indicator     ::mixc::docker_bit_indicator
