#ifndef xpack_docker_bit_indicator
#define xpack_docker_bit_indicator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_bit_indicator
#include"docker/private/adapter.bit_indicator.hpp"
#include"dumb/disable_copy.hpp"
#include"interface/can_alloc.hpp"
#include"lock/atom_swap.hpp"
#include"memop/zeros.hpp"
#include"memory/allocator.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_hashmap{
    template<class key_t, class val_t> struct hashmap_t;
}

namespace mixc::docker_bit_indicator{
    // bits != 0 时该模板是使用静态内存分配
    template<uxx bits = 0>
    xstruct(
        xtmpl(bit_indicator_t, bits)
    )
    public:
        constexpr uxx length(){
            return bits;
        }
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
        static const uxx * level_lut(){
            return lut;
        }

        static constexpr uxx size(){
            return lv0 + lv1 + lv2 + lv3 + lv4 + lv5 + lv6 + lv7 + lv8 + lv9;
        }

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

        uxx * bmp(){
            return data;
        }

        uxx data[size()] = {0};
    $

    // 动态内存分配的位图
    template<>
    xstruct(
        xspec(bit_indicator_t),
        xpubb(inc::disable_copy),
        xprif(pbmp      , uxx *),
        xprif(pheight   , uxx),
        xprif(psize     , uxx),
        xprif(pbits     , uxx)
    )
        template<class key_t, class val_t> friend struct mixc::docker_hashmap::hashmap_t;

        using final = the_t;

        bit_indicator_t() : 
            pbmp(nullptr), pheight(0), psize(0) {}

        bit_indicator_t(uxx bits, inc::can_alloc<uxx> alloc){
            uxx buf[sizeof(uxx) * 8 / inc::bwidth + 1];

            // 先设置，之后 bits 会被修改
            the.bits(bits);
            the.pheight = 0;
            the.psize   = 0;

            do{
                bits               = bits / inc::bwidth + (bits % inc::bwidth != 0);
                buf[the.pheight]   = bits;
                the.pheight       += 1;
                the.psize         += bits;
            }while(bits > 1);

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

        bit_indicator_t(uxx bits) : 
            bit_indicator_t(bits, [](uxx length){
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

        xproget(height)
        xproget_prisetx(size, uxx)
            xr{ return the.psize >> 1; }
            xw{ 
                the.psize &= 1;
                the.psize |= value << 1;
            }

        xproget_prisetx(bmp, uxx *)
            xr{ return the.pbmp; }
            xw{ the.pbmp = value + the.height(); }

        xpriget_prisetx(need_free, bool)
            xr{ return the.psize & 1; }
            xw{ the.psize = value ? the.psize | 1 : the.psize & (uxx(-1) << 1); }

        // 位图是分层的，在 64bit 系统，上层用 1bit 表示下层 64bit 位组中是否有置位位
        // 该属性存放了每次层一共有多少个字
        xprogetx(level_lut, uxx *){
            return the.pbmp - the.height();
        }

        // 动态内存部分一共有多少个字
        xprigetx(cost_count, uxx){
            return the.size() + the.height();
        }

        xprigetx(cost_bytes, inc::memory_size){
            return inc::memory_size(cost_count() * sizeof(uxx));
        }

        xpubget_priset(bits);
    $

    template<class final, uxx bits>
    using bit_indicator = inc::adapter_bit_indicator<final, bit_indicator_t<bits>>;
}
#endif

#define xusing_docker_bit_indicator     ::mixc::docker_bit_indicator
