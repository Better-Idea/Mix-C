/*
代码生成
================================================================================================
#include<stdio.h>

void make_xlist(int count){
    printf("#define __xlist_core__(first,get,");

    for (int i = 0; i < count; i++){
        printf("a%d,", i);
    }

    printf("...)    \\\n");
    printf("__xprefix_ ## first ## a0    \\\n");

    for (int i = 1; i < count; i++){
        printf("__xprefix_ ## get    ## a%d    \\\n", i);
    }

    printf("\n\n");
}

int main(){
    make_xlist(64);
    return 0;
}
*/

#pragma once
//#include"macro/xlink.hpp"
#define __xlist__(first,get,...)         __xlist_core__(first,get,__VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)
#define __xlist_core__(first,get,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32,a33,a34,a35,a36,a37,a38,a39,a40,a41,a42,a43,a44,a45,a46,a47,a48,a49,a50,a51,a52,a53,a54,a55,a56,a57,a58,a59,a60,a61,a62,a63,...)    \
__xprefix_ ## first ## a0     \
__xprefix_ ## get    ## a1    \
__xprefix_ ## get    ## a2    \
__xprefix_ ## get    ## a3    \
__xprefix_ ## get    ## a4    \
__xprefix_ ## get    ## a5    \
__xprefix_ ## get    ## a6    \
__xprefix_ ## get    ## a7    \
__xprefix_ ## get    ## a8    \
__xprefix_ ## get    ## a9    \
__xprefix_ ## get    ## a10    \
__xprefix_ ## get    ## a11    \
__xprefix_ ## get    ## a12    \
__xprefix_ ## get    ## a13    \
__xprefix_ ## get    ## a14    \
__xprefix_ ## get    ## a15    \
__xprefix_ ## get    ## a16    \
__xprefix_ ## get    ## a17    \
__xprefix_ ## get    ## a18    \
__xprefix_ ## get    ## a19    \
__xprefix_ ## get    ## a20    \
__xprefix_ ## get    ## a21    \
__xprefix_ ## get    ## a22    \
__xprefix_ ## get    ## a23    \
__xprefix_ ## get    ## a24    \
__xprefix_ ## get    ## a25    \
__xprefix_ ## get    ## a26    \
__xprefix_ ## get    ## a27    \
__xprefix_ ## get    ## a28    \
__xprefix_ ## get    ## a29    \
__xprefix_ ## get    ## a30    \
__xprefix_ ## get    ## a31    \
__xprefix_ ## get    ## a32    \
__xprefix_ ## get    ## a33    \
__xprefix_ ## get    ## a34    \
__xprefix_ ## get    ## a35    \
__xprefix_ ## get    ## a36    \
__xprefix_ ## get    ## a37    \
__xprefix_ ## get    ## a38    \
__xprefix_ ## get    ## a39    \
__xprefix_ ## get    ## a40    \
__xprefix_ ## get    ## a41    \
__xprefix_ ## get    ## a42    \
__xprefix_ ## get    ## a43    \
__xprefix_ ## get    ## a44    \
__xprefix_ ## get    ## a45    \
__xprefix_ ## get    ## a46    \
__xprefix_ ## get    ## a47    \
__xprefix_ ## get    ## a48    \
__xprefix_ ## get    ## a49    \
__xprefix_ ## get    ## a50    \
__xprefix_ ## get    ## a51    \
__xprefix_ ## get    ## a52    \
__xprefix_ ## get    ## a53    \
__xprefix_ ## get    ## a54    \
__xprefix_ ## get    ## a55    \
__xprefix_ ## get    ## a56    \
__xprefix_ ## get    ## a57    \
__xprefix_ ## get    ## a58    \
__xprefix_ ## get    ## a59    \
__xprefix_ ## get    ## a60    \
__xprefix_ ## get    ## a61    \
__xprefix_ ## get    ## a62    \
__xprefix_ ## get    ## a63    \

