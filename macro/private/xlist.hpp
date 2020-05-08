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
#include"macro/xlink.hpp"
#define __xlist__(first,get,...)         __xlist_core__(first,get,__VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)
#define __xlist_core__(first,get,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32,a33,a34,a35,a36,a37,a38,a39,a40,a41,a42,a43,a44,a45,a46,a47,a48,a49,a50,a51,a52,a53,a54,a55,a56,a57,a58,a59,a60,a61,a62,a63,...)    \
xlink2(__xprefix_, xlink2(first, a0 ))    \
xlink2(__xprefix_, xlink2(get  , a1 ))    \
xlink2(__xprefix_, xlink2(get  , a2 ))    \
xlink2(__xprefix_, xlink2(get  , a3 ))    \
xlink2(__xprefix_, xlink2(get  , a4 ))    \
xlink2(__xprefix_, xlink2(get  , a5 ))    \
xlink2(__xprefix_, xlink2(get  , a6 ))    \
xlink2(__xprefix_, xlink2(get  , a7 ))    \
xlink2(__xprefix_, xlink2(get  , a8 ))    \
xlink2(__xprefix_, xlink2(get  , a9 ))    \
xlink2(__xprefix_, xlink2(get  , a10))    \
xlink2(__xprefix_, xlink2(get  , a11))    \
xlink2(__xprefix_, xlink2(get  , a12))    \
xlink2(__xprefix_, xlink2(get  , a13))    \
xlink2(__xprefix_, xlink2(get  , a14))    \
xlink2(__xprefix_, xlink2(get  , a15))    \
xlink2(__xprefix_, xlink2(get  , a16))    \
xlink2(__xprefix_, xlink2(get  , a17))    \
xlink2(__xprefix_, xlink2(get  , a18))    \
xlink2(__xprefix_, xlink2(get  , a19))    \
xlink2(__xprefix_, xlink2(get  , a20))    \
xlink2(__xprefix_, xlink2(get  , a21))    \
xlink2(__xprefix_, xlink2(get  , a22))    \
xlink2(__xprefix_, xlink2(get  , a23))    \
xlink2(__xprefix_, xlink2(get  , a24))    \
xlink2(__xprefix_, xlink2(get  , a25))    \
xlink2(__xprefix_, xlink2(get  , a26))    \
xlink2(__xprefix_, xlink2(get  , a27))    \
xlink2(__xprefix_, xlink2(get  , a28))    \
xlink2(__xprefix_, xlink2(get  , a29))    \
xlink2(__xprefix_, xlink2(get  , a30))    \
xlink2(__xprefix_, xlink2(get  , a31))    \
xlink2(__xprefix_, xlink2(get  , a32))    \
xlink2(__xprefix_, xlink2(get  , a33))    \
xlink2(__xprefix_, xlink2(get  , a34))    \
xlink2(__xprefix_, xlink2(get  , a35))    \
xlink2(__xprefix_, xlink2(get  , a36))    \
xlink2(__xprefix_, xlink2(get  , a37))    \
xlink2(__xprefix_, xlink2(get  , a38))    \
xlink2(__xprefix_, xlink2(get  , a39))    \
xlink2(__xprefix_, xlink2(get  , a40))    \
xlink2(__xprefix_, xlink2(get  , a41))    \
xlink2(__xprefix_, xlink2(get  , a42))    \
xlink2(__xprefix_, xlink2(get  , a43))    \
xlink2(__xprefix_, xlink2(get  , a44))    \
xlink2(__xprefix_, xlink2(get  , a45))    \
xlink2(__xprefix_, xlink2(get  , a46))    \
xlink2(__xprefix_, xlink2(get  , a47))    \
xlink2(__xprefix_, xlink2(get  , a48))    \
xlink2(__xprefix_, xlink2(get  , a49))    \
xlink2(__xprefix_, xlink2(get  , a50))    \
xlink2(__xprefix_, xlink2(get  , a51))    \
xlink2(__xprefix_, xlink2(get  , a52))    \
xlink2(__xprefix_, xlink2(get  , a53))    \
xlink2(__xprefix_, xlink2(get  , a54))    \
xlink2(__xprefix_, xlink2(get  , a55))    \
xlink2(__xprefix_, xlink2(get  , a56))    \
xlink2(__xprefix_, xlink2(get  , a57))    \
xlink2(__xprefix_, xlink2(get  , a58))    \
xlink2(__xprefix_, xlink2(get  , a59))    \
xlink2(__xprefix_, xlink2(get  , a60))    \
xlink2(__xprefix_, xlink2(get  , a61))    \
xlink2(__xprefix_, xlink2(get  , a62))    \
xlink2(__xprefix_, xlink2(get  , a63))    \

