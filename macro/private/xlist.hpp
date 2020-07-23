#pragma once
#include"macro/xlink.hpp"

#define __xlist_core__(get0,get1,get2,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32,a33,a34,a35,a36,a37,a38,a39,a40,a41,a42,a43,a44,a45,a46,a47,a48,a49,a50,a51,a52,a53,a54,a55,a56,a57,a58,a59,a60,a61,a62,a63,...)    \
get0 ## a0  get1 ## a1  get2 ## a2  get2 ## a3  get2 ## a4  get2 ## a5  get2 ## a6  get2 ## a7  get2 ## a8  get2 ## a9  get2 ## a10 get2 ## a11 get2 ## a12 get2 ## a13 get2 ## a14 get2 ## a15 get2 ## a16 get2 ## a17 get2 ## a18 get2 ## a19 get2 ## a20 get2 ## a21 get2 ## a22 get2 ## a23 get2 ## a24 get2 ## a25 get2 ## a26 get2 ## a27 get2 ## a28 get2 ## a29 get2 ## a30 get2 ## a31 get2 ## a32 get2 ## a33 get2 ## a34 get2 ## a35 get2 ## a36 get2 ## a37 get2 ## a38 get2 ## a39 get2 ## a40 get2 ## a41 get2 ## a42 get2 ## a43 get2 ## a44 get2 ## a45 get2 ## a46 get2 ## a47 get2 ## a48 get2 ## a49 get2 ## a50 get2 ## a51 get2 ## a52 get2 ## a53 get2 ## a54 get2 ## a55 get2 ## a56 get2 ## a57 get2 ## a58 get2 ## a59 get2 ## a60 get2 ## a61 get2 ## a62 get2 ## a63
#define __xlist3__(get0,get1,get2,...)  xlink2(__xlist, _core__(get0,get1,get2,__VA_ARGS__,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_))
#define __xlist2__(get0,get1,...)       __xlist3__(get0,get1,get1,__VA_ARGS__)
#define __xlist__(get,...)              __xlist3__(get,get,get,__VA_ARGS__)
