#ifndef xpack_talk_about_allocator
#define xpack_talk_about_allocator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::talk_about_allocator::inc
#include"macro/xhint.hpp"
#include"docker/array.hpp"
#include"math/random.hpp"
#include"memory/allocator.hpp"
#include"memop/fill.hpp"
#pragma pop_macro("xuser")

namespace mixc::talk_about_allocator::inc{
    void test(){
        struct pair{ u08p ptr; uxx size; };
        static array<pair, 1024 * 100> record;
        xhint(used_bytes());

        for(uxx i = 0; i < record.length(); ){
            auto mode           = uxx(random<u08>() % (3));
            auto bytes          = uxx(random<u16>() % (64 * 64) * 16);
            auto pr             = pair();
            auto pt             = pair();

            if (bytes == 0){
                bytes           = 16;
            }

            if (mode == 0 and i != 0){
                auto index      = uxx(random<uxx>() % i);
                pt              = record[index];
                record[index]   = record[--i];
                inc::free(pt.ptr, memory_size{pt.size});
            }
            else{
                pr.ptr          = inc::alloc<u08>(memory_size{bytes});
                pr.size         = bytes;
                record[i++]     = pr;
                inc::fill(pr.ptr, 0xff, bytes); // 写脏
            }
        }

        xhint(used_bytes());

        for(uxx i = 0; i < record.length(); i++){
            auto pr             = record[i];
            inc::free(pr.ptr, memory_size{pr.size});
        }
        xhint(used_bytes());
    }
}

int main(){
    mixc::talk_about_allocator::inc::test();
    return 0;
}

#endif
