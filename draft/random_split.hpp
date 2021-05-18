#define xuser mixc::powerful_cat::inc
#include"math/random.hpp"
#include"interface/ranger.hpp"

namespace mixc::powerful_cat{
    inline bstate_t random_split(inc::ranger<uxx> list, uxx total){
        uxx length      = list.length();

        if (total < length or length == 0){
            return bstate_t::fail;
        }

        uxx average     = total / length;
        uxx first       = total - length * average;

        for(uxx i = 1; i < length; i++){
            list[i]     = average;
        }

        list[0]         = first;
        
        for(uxx i = 0; i < length; i++){
            uxx left    = inc::random<uxx>() % length;
            uxx right   = inc::random<uxx>() % length;
            uxx rest    = list[left];

            if (rest == 1){
                continue;
            }

            uxx value   = inc::random<uxx>() % (rest - 1) + 1;
            list[left] -= value;
            list[right]+= value;
        }
        return bstate_t::success;
    }
}