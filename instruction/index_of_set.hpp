#pragma once
#include"define/base_type.hpp"
#include"define/platform.hpp"

namespace mixc{
    inline uxx index_of_set(u32 v){
        uxx i = v;
        uxx n = not_exist;
        asm("bsf %0, %0":"=r"(i));
        asm("cmovz %1, %2":"=a"(i):"r"(n),"a"(i));
        return i;
    }

    inline uxx index_of_set(u64 v){
        uxx i = uxx(v);
        uxx n = not_exist;

        if constexpr (is_64bit){
            asm("bsf %0, %0":"=r"(i));
            asm("cmovz %1, %2":"=a"(i):"r"(n),"a"(i));
        }
        else{
            asm("bsf %0, %0":"=r"(i));
            asm("jz pass");
            asm("bsf %1, %0":"=r"(i):"r"(u32(v >> 32)));
            asm("bts $5, %0":"=r"(i));
            asm("pass:");
        }
        return i;
    }
}