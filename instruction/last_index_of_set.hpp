#pragma once
#include"define/base_type.hpp"
#include"define/platform.hpp"

namespace mixc{
    inline uxx last_index_of_set(u32 v){
        uxx i = v;
        uxx n = not_exist;
        asm("bsr %0, %0":"=r"(i));
        asm("cmovz %1, %2":"=a"(i):"r"(n),"a"(i));
        return i;
    }

    inline uxx last_index_of_set(u64 v){
        uxx n = not_exist;
        uxx i;

        if constexpr (is_64bit){
            i = uxx(v);
            asm("bsr %0, %0":"=r"(i));
            asm("cmovz %1, %2":"=a"(i):"r"(n),"a"(i));
        }
        else{
            i = uxx(v >> 32);
            asm("bsf %0, %0":"=a"(i));
            asm("jz pass");
            asm("bts $5, %0":"=a"(i));
            asm("ret");
            asm("pass:");
            asm("bsf %1, %0":"=r"(i):"r"(u32(v >> 32)));
        }
        return i;
    }
}