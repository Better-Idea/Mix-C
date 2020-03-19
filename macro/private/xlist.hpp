/*
代码生成
================================================================================================
#include<stdio.h>

void make_xlist(int count){
    printf("#define __xlist_core__(xheader,xget,");
    
    for (int i = 0; i < count; i++){
        printf("a%d,", i);
    }

    printf("...)    \\\n");
    printf("__xprefix_ ## xheader ## a0    \\\n");
    
    for (int i = 1; i < count; i++){
         printf("__xprefix_ ## xget    ## a%d    \\\n", i);
    }

    printf("\n\n");
}

int main(){
    make_xlist(64);
    return 0;
}
*/

#pragma once
#define __xlist__(xheader,xget,...)         __xlist_core__(xheader,xget,__VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)
#define __xlist_core__(xheader,xget,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32,a33,a34,a35,a36,a37,a38,a39,a40,a41,a42,a43,a44,a45,a46,a47,a48,a49,a50,a51,a52,a53,a54,a55,a56,a57,a58,a59,a60,a61,a62,a63,...)    \
__xprefix_ ## xheader ## a0    \
__xprefix_ ## xget    ## a1    \
__xprefix_ ## xget    ## a2    \
__xprefix_ ## xget    ## a3    \
__xprefix_ ## xget    ## a4    \
__xprefix_ ## xget    ## a5    \
__xprefix_ ## xget    ## a6    \
__xprefix_ ## xget    ## a7    \
__xprefix_ ## xget    ## a8    \
__xprefix_ ## xget    ## a9    \
__xprefix_ ## xget    ## a10    \
__xprefix_ ## xget    ## a11    \
__xprefix_ ## xget    ## a12    \
__xprefix_ ## xget    ## a13    \
__xprefix_ ## xget    ## a14    \
__xprefix_ ## xget    ## a15    \
__xprefix_ ## xget    ## a16    \
__xprefix_ ## xget    ## a17    \
__xprefix_ ## xget    ## a18    \
__xprefix_ ## xget    ## a19    \
__xprefix_ ## xget    ## a20    \
__xprefix_ ## xget    ## a21    \
__xprefix_ ## xget    ## a22    \
__xprefix_ ## xget    ## a23    \
__xprefix_ ## xget    ## a24    \
__xprefix_ ## xget    ## a25    \
__xprefix_ ## xget    ## a26    \
__xprefix_ ## xget    ## a27    \
__xprefix_ ## xget    ## a28    \
__xprefix_ ## xget    ## a29    \
__xprefix_ ## xget    ## a30    \
__xprefix_ ## xget    ## a31    \
__xprefix_ ## xget    ## a32    \
__xprefix_ ## xget    ## a33    \
__xprefix_ ## xget    ## a34    \
__xprefix_ ## xget    ## a35    \
__xprefix_ ## xget    ## a36    \
__xprefix_ ## xget    ## a37    \
__xprefix_ ## xget    ## a38    \
__xprefix_ ## xget    ## a39    \
__xprefix_ ## xget    ## a40    \
__xprefix_ ## xget    ## a41    \
__xprefix_ ## xget    ## a42    \
__xprefix_ ## xget    ## a43    \
__xprefix_ ## xget    ## a44    \
__xprefix_ ## xget    ## a45    \
__xprefix_ ## xget    ## a46    \
__xprefix_ ## xget    ## a47    \
__xprefix_ ## xget    ## a48    \
__xprefix_ ## xget    ## a49    \
__xprefix_ ## xget    ## a50    \
__xprefix_ ## xget    ## a51    \
__xprefix_ ## xget    ## a52    \
__xprefix_ ## xget    ## a53    \
__xprefix_ ## xget    ## a54    \
__xprefix_ ## xget    ## a55    \
__xprefix_ ## xget    ## a56    \
__xprefix_ ## xget    ## a57    \
__xprefix_ ## xget    ## a58    \
__xprefix_ ## xget    ## a59    \
__xprefix_ ## xget    ## a60    \
__xprefix_ ## xget    ## a61    \
__xprefix_ ## xget    ## a62    \
__xprefix_ ## xget    ## a63    \

