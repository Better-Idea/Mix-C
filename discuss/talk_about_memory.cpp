#define xuser mixc::talk_about_allocator::inc
#include"macro/xhint.hpp"
#include"docker/array.hpp"
#include"math/random.hpp"
#include"memop/fill.hpp"
#include"mixc.hpp"
#include"utils/memory.hpp"

xinit(xuser::the_main){
    using namespace xuser;
    struct pair{ u08p ptr; uxx size; };
    static array<pair, 1024 * 100> record;
    xhint(memory::used_bytes());

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
            xuser::memory::free(pt.ptr, memory::size{pt.size});
        }
        else{
            pr.ptr          = xuser::memory::alloc<u08>(memory::size{bytes});
            pr.size         = bytes;
            record[i++]     = pr;
            xuser::fill(pr.ptr, 0xff, bytes); // 写脏
        }
    }

    xhint(memory::used_bytes());

    for(uxx i = 0; i < record.length(); i++){
        auto pr             = record[i];
        xuser::memory::free(pr.ptr, memory::size{pr.size});
    }

    xhint(memory::used_bytes());
};
