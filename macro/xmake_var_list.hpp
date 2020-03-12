#pragma once
/*
#include<stdio.h>

int count = 64; // 设置参数组数 (m, t, n)

void gen(int v){
    if (v == 0)         printf("#define __xmake_var_list_type_name__(");
    else if (v == 1)    printf("#define __xmake_var_list_name__(");
    else if (v == 2)    printf("#define __xmake_var_list_type__(");
    else if (v == 3)    printf("#define __xmake_var_list_check__(");

    for (int i = 0; i < count; i++){
        printf("m%d,t%d,n%d,", i, i, i);
    }

    printf("...)   \\\n");
    if (v == 0)         printf("    t0 n0\\\n");
    else if (v == 1)    printf("    n0\\\n");
    else if (v == 2)    printf("    t0\\\n");
    
    for (int i = 1; i < count; i++){
        if (v == 0)         printf("    xlink2(m%d, xdummy)(t%d n%d) \\\n", i, i, i);
        else if (v == 1)    printf("    xlink2(m%d, xdummy)(n%d) \\\n", i, i);
        else if (v == 2)    printf("    xlink2(m%d, xdummy)(t%d) \\\n", i, i);
    }

    if (v == 3){
        printf("    static_assert(# __VA_ARGS__[0] == ',', \"xmake_var_list_xxx param count overflow, you need remake this macro by the script which in [macro/xmake_var_list.hpp]\");");
    }
    printf("\n");

    if (v == 0)         printf("#define xmake_var_list_type_name(...)   __xmake_var_list_type_name__(__VA_ARGS__,");
    else if (v == 1)    printf("#define xmake_var_list_name(...)        __xmake_var_list_name__(__VA_ARGS__,");
    else if (v == 2)    printf("#define xmake_var_list_type(...)        __xmake_var_list_type__(__VA_ARGS__,");
    else if (v == 3)    printf("#define xmake_var_list_check(...)       __xmake_var_list_check__(__VA_ARGS__,");

    for (int i = 0; i < count; i++){
        printf(",,,");
    }

    printf(")\n\n");
}

int main(){
    gen(0);
    gen(1);
    gen(2);
    gen(3);
    return 0;
}
*/

#include"macro/xdummy.hpp"
#include"macro/xlink.hpp"
#define xarg(...)          __VA_ARGS__
#define xargxdummy(...)    , __VA_ARGS__

#define __xmake_var_list_type_name__(m0,t0,n0,m1,t1,n1,m2,t2,n2,m3,t3,n3,m4,t4,n4,m5,t5,n5,m6,t6,n6,m7,t7,n7,m8,t8,n8,m9,t9,n9,m10,t10,n10,m11,t11,n11,m12,t12,n12,m13,t13,n13,m14,t14,n14,m15,t15,n15,m16,t16,n16,m17,t17,n17,m18,t18,n18,m19,t19,n19,m20,t20,n20,m21,t21,n21,m22,t22,n22,m23,t23,n23,m24,t24,n24,m25,t25,n25,m26,t26,n26,m27,t27,n27,m28,t28,n28,m29,t29,n29,m30,t30,n30,m31,t31,n31,m32,t32,n32,m33,t33,n33,m34,t34,n34,m35,t35,n35,m36,t36,n36,m37,t37,n37,m38,t38,n38,m39,t39,n39,m40,t40,n40,m41,t41,n41,m42,t42,n42,m43,t43,n43,m44,t44,n44,m45,t45,n45,m46,t46,n46,m47,t47,n47,m48,t48,n48,m49,t49,n49,m50,t50,n50,m51,t51,n51,m52,t52,n52,m53,t53,n53,m54,t54,n54,m55,t55,n55,m56,t56,n56,m57,t57,n57,m58,t58,n58,m59,t59,n59,m60,t60,n60,m61,t61,n61,m62,t62,n62,m63,t63,n63,...)   \
    t0 n0\
    xlink2(m1, xdummy)(t1 n1) \
    xlink2(m2, xdummy)(t2 n2) \
    xlink2(m3, xdummy)(t3 n3) \
    xlink2(m4, xdummy)(t4 n4) \
    xlink2(m5, xdummy)(t5 n5) \
    xlink2(m6, xdummy)(t6 n6) \
    xlink2(m7, xdummy)(t7 n7) \
    xlink2(m8, xdummy)(t8 n8) \
    xlink2(m9, xdummy)(t9 n9) \
    xlink2(m10, xdummy)(t10 n10) \
    xlink2(m11, xdummy)(t11 n11) \
    xlink2(m12, xdummy)(t12 n12) \
    xlink2(m13, xdummy)(t13 n13) \
    xlink2(m14, xdummy)(t14 n14) \
    xlink2(m15, xdummy)(t15 n15) \
    xlink2(m16, xdummy)(t16 n16) \
    xlink2(m17, xdummy)(t17 n17) \
    xlink2(m18, xdummy)(t18 n18) \
    xlink2(m19, xdummy)(t19 n19) \
    xlink2(m20, xdummy)(t20 n20) \
    xlink2(m21, xdummy)(t21 n21) \
    xlink2(m22, xdummy)(t22 n22) \
    xlink2(m23, xdummy)(t23 n23) \
    xlink2(m24, xdummy)(t24 n24) \
    xlink2(m25, xdummy)(t25 n25) \
    xlink2(m26, xdummy)(t26 n26) \
    xlink2(m27, xdummy)(t27 n27) \
    xlink2(m28, xdummy)(t28 n28) \
    xlink2(m29, xdummy)(t29 n29) \
    xlink2(m30, xdummy)(t30 n30) \
    xlink2(m31, xdummy)(t31 n31) \
    xlink2(m32, xdummy)(t32 n32) \
    xlink2(m33, xdummy)(t33 n33) \
    xlink2(m34, xdummy)(t34 n34) \
    xlink2(m35, xdummy)(t35 n35) \
    xlink2(m36, xdummy)(t36 n36) \
    xlink2(m37, xdummy)(t37 n37) \
    xlink2(m38, xdummy)(t38 n38) \
    xlink2(m39, xdummy)(t39 n39) \
    xlink2(m40, xdummy)(t40 n40) \
    xlink2(m41, xdummy)(t41 n41) \
    xlink2(m42, xdummy)(t42 n42) \
    xlink2(m43, xdummy)(t43 n43) \
    xlink2(m44, xdummy)(t44 n44) \
    xlink2(m45, xdummy)(t45 n45) \
    xlink2(m46, xdummy)(t46 n46) \
    xlink2(m47, xdummy)(t47 n47) \
    xlink2(m48, xdummy)(t48 n48) \
    xlink2(m49, xdummy)(t49 n49) \
    xlink2(m50, xdummy)(t50 n50) \
    xlink2(m51, xdummy)(t51 n51) \
    xlink2(m52, xdummy)(t52 n52) \
    xlink2(m53, xdummy)(t53 n53) \
    xlink2(m54, xdummy)(t54 n54) \
    xlink2(m55, xdummy)(t55 n55) \
    xlink2(m56, xdummy)(t56 n56) \
    xlink2(m57, xdummy)(t57 n57) \
    xlink2(m58, xdummy)(t58 n58) \
    xlink2(m59, xdummy)(t59 n59) \
    xlink2(m60, xdummy)(t60 n60) \
    xlink2(m61, xdummy)(t61 n61) \
    xlink2(m62, xdummy)(t62 n62) \
    xlink2(m63, xdummy)(t63 n63) \

#define xmake_var_list_type_name(...)   __xmake_var_list_type_name__(__VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)

#define __xmake_var_list_name__(m0,t0,n0,m1,t1,n1,m2,t2,n2,m3,t3,n3,m4,t4,n4,m5,t5,n5,m6,t6,n6,m7,t7,n7,m8,t8,n8,m9,t9,n9,m10,t10,n10,m11,t11,n11,m12,t12,n12,m13,t13,n13,m14,t14,n14,m15,t15,n15,m16,t16,n16,m17,t17,n17,m18,t18,n18,m19,t19,n19,m20,t20,n20,m21,t21,n21,m22,t22,n22,m23,t23,n23,m24,t24,n24,m25,t25,n25,m26,t26,n26,m27,t27,n27,m28,t28,n28,m29,t29,n29,m30,t30,n30,m31,t31,n31,m32,t32,n32,m33,t33,n33,m34,t34,n34,m35,t35,n35,m36,t36,n36,m37,t37,n37,m38,t38,n38,m39,t39,n39,m40,t40,n40,m41,t41,n41,m42,t42,n42,m43,t43,n43,m44,t44,n44,m45,t45,n45,m46,t46,n46,m47,t47,n47,m48,t48,n48,m49,t49,n49,m50,t50,n50,m51,t51,n51,m52,t52,n52,m53,t53,n53,m54,t54,n54,m55,t55,n55,m56,t56,n56,m57,t57,n57,m58,t58,n58,m59,t59,n59,m60,t60,n60,m61,t61,n61,m62,t62,n62,m63,t63,n63,...)   \
    n0\
    xlink2(m1, xdummy)(n1) \
    xlink2(m2, xdummy)(n2) \
    xlink2(m3, xdummy)(n3) \
    xlink2(m4, xdummy)(n4) \
    xlink2(m5, xdummy)(n5) \
    xlink2(m6, xdummy)(n6) \
    xlink2(m7, xdummy)(n7) \
    xlink2(m8, xdummy)(n8) \
    xlink2(m9, xdummy)(n9) \
    xlink2(m10, xdummy)(n10) \
    xlink2(m11, xdummy)(n11) \
    xlink2(m12, xdummy)(n12) \
    xlink2(m13, xdummy)(n13) \
    xlink2(m14, xdummy)(n14) \
    xlink2(m15, xdummy)(n15) \
    xlink2(m16, xdummy)(n16) \
    xlink2(m17, xdummy)(n17) \
    xlink2(m18, xdummy)(n18) \
    xlink2(m19, xdummy)(n19) \
    xlink2(m20, xdummy)(n20) \
    xlink2(m21, xdummy)(n21) \
    xlink2(m22, xdummy)(n22) \
    xlink2(m23, xdummy)(n23) \
    xlink2(m24, xdummy)(n24) \
    xlink2(m25, xdummy)(n25) \
    xlink2(m26, xdummy)(n26) \
    xlink2(m27, xdummy)(n27) \
    xlink2(m28, xdummy)(n28) \
    xlink2(m29, xdummy)(n29) \
    xlink2(m30, xdummy)(n30) \
    xlink2(m31, xdummy)(n31) \
    xlink2(m32, xdummy)(n32) \
    xlink2(m33, xdummy)(n33) \
    xlink2(m34, xdummy)(n34) \
    xlink2(m35, xdummy)(n35) \
    xlink2(m36, xdummy)(n36) \
    xlink2(m37, xdummy)(n37) \
    xlink2(m38, xdummy)(n38) \
    xlink2(m39, xdummy)(n39) \
    xlink2(m40, xdummy)(n40) \
    xlink2(m41, xdummy)(n41) \
    xlink2(m42, xdummy)(n42) \
    xlink2(m43, xdummy)(n43) \
    xlink2(m44, xdummy)(n44) \
    xlink2(m45, xdummy)(n45) \
    xlink2(m46, xdummy)(n46) \
    xlink2(m47, xdummy)(n47) \
    xlink2(m48, xdummy)(n48) \
    xlink2(m49, xdummy)(n49) \
    xlink2(m50, xdummy)(n50) \
    xlink2(m51, xdummy)(n51) \
    xlink2(m52, xdummy)(n52) \
    xlink2(m53, xdummy)(n53) \
    xlink2(m54, xdummy)(n54) \
    xlink2(m55, xdummy)(n55) \
    xlink2(m56, xdummy)(n56) \
    xlink2(m57, xdummy)(n57) \
    xlink2(m58, xdummy)(n58) \
    xlink2(m59, xdummy)(n59) \
    xlink2(m60, xdummy)(n60) \
    xlink2(m61, xdummy)(n61) \
    xlink2(m62, xdummy)(n62) \
    xlink2(m63, xdummy)(n63) \

#define xmake_var_list_name(...)        __xmake_var_list_name__(__VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)

#define __xmake_var_list_type__(m0,t0,n0,m1,t1,n1,m2,t2,n2,m3,t3,n3,m4,t4,n4,m5,t5,n5,m6,t6,n6,m7,t7,n7,m8,t8,n8,m9,t9,n9,m10,t10,n10,m11,t11,n11,m12,t12,n12,m13,t13,n13,m14,t14,n14,m15,t15,n15,m16,t16,n16,m17,t17,n17,m18,t18,n18,m19,t19,n19,m20,t20,n20,m21,t21,n21,m22,t22,n22,m23,t23,n23,m24,t24,n24,m25,t25,n25,m26,t26,n26,m27,t27,n27,m28,t28,n28,m29,t29,n29,m30,t30,n30,m31,t31,n31,m32,t32,n32,m33,t33,n33,m34,t34,n34,m35,t35,n35,m36,t36,n36,m37,t37,n37,m38,t38,n38,m39,t39,n39,m40,t40,n40,m41,t41,n41,m42,t42,n42,m43,t43,n43,m44,t44,n44,m45,t45,n45,m46,t46,n46,m47,t47,n47,m48,t48,n48,m49,t49,n49,m50,t50,n50,m51,t51,n51,m52,t52,n52,m53,t53,n53,m54,t54,n54,m55,t55,n55,m56,t56,n56,m57,t57,n57,m58,t58,n58,m59,t59,n59,m60,t60,n60,m61,t61,n61,m62,t62,n62,m63,t63,n63,...)   \
    t0\
    xlink2(m1, xdummy)(t1) \
    xlink2(m2, xdummy)(t2) \
    xlink2(m3, xdummy)(t3) \
    xlink2(m4, xdummy)(t4) \
    xlink2(m5, xdummy)(t5) \
    xlink2(m6, xdummy)(t6) \
    xlink2(m7, xdummy)(t7) \
    xlink2(m8, xdummy)(t8) \
    xlink2(m9, xdummy)(t9) \
    xlink2(m10, xdummy)(t10) \
    xlink2(m11, xdummy)(t11) \
    xlink2(m12, xdummy)(t12) \
    xlink2(m13, xdummy)(t13) \
    xlink2(m14, xdummy)(t14) \
    xlink2(m15, xdummy)(t15) \
    xlink2(m16, xdummy)(t16) \
    xlink2(m17, xdummy)(t17) \
    xlink2(m18, xdummy)(t18) \
    xlink2(m19, xdummy)(t19) \
    xlink2(m20, xdummy)(t20) \
    xlink2(m21, xdummy)(t21) \
    xlink2(m22, xdummy)(t22) \
    xlink2(m23, xdummy)(t23) \
    xlink2(m24, xdummy)(t24) \
    xlink2(m25, xdummy)(t25) \
    xlink2(m26, xdummy)(t26) \
    xlink2(m27, xdummy)(t27) \
    xlink2(m28, xdummy)(t28) \
    xlink2(m29, xdummy)(t29) \
    xlink2(m30, xdummy)(t30) \
    xlink2(m31, xdummy)(t31) \
    xlink2(m32, xdummy)(t32) \
    xlink2(m33, xdummy)(t33) \
    xlink2(m34, xdummy)(t34) \
    xlink2(m35, xdummy)(t35) \
    xlink2(m36, xdummy)(t36) \
    xlink2(m37, xdummy)(t37) \
    xlink2(m38, xdummy)(t38) \
    xlink2(m39, xdummy)(t39) \
    xlink2(m40, xdummy)(t40) \
    xlink2(m41, xdummy)(t41) \
    xlink2(m42, xdummy)(t42) \
    xlink2(m43, xdummy)(t43) \
    xlink2(m44, xdummy)(t44) \
    xlink2(m45, xdummy)(t45) \
    xlink2(m46, xdummy)(t46) \
    xlink2(m47, xdummy)(t47) \
    xlink2(m48, xdummy)(t48) \
    xlink2(m49, xdummy)(t49) \
    xlink2(m50, xdummy)(t50) \
    xlink2(m51, xdummy)(t51) \
    xlink2(m52, xdummy)(t52) \
    xlink2(m53, xdummy)(t53) \
    xlink2(m54, xdummy)(t54) \
    xlink2(m55, xdummy)(t55) \
    xlink2(m56, xdummy)(t56) \
    xlink2(m57, xdummy)(t57) \
    xlink2(m58, xdummy)(t58) \
    xlink2(m59, xdummy)(t59) \
    xlink2(m60, xdummy)(t60) \
    xlink2(m61, xdummy)(t61) \
    xlink2(m62, xdummy)(t62) \
    xlink2(m63, xdummy)(t63) \

#define xmake_var_list_type(...)        __xmake_var_list_type__(__VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)

#define __xmake_var_list_check__(m0,t0,n0,m1,t1,n1,m2,t2,n2,m3,t3,n3,m4,t4,n4,m5,t5,n5,m6,t6,n6,m7,t7,n7,m8,t8,n8,m9,t9,n9,m10,t10,n10,m11,t11,n11,m12,t12,n12,m13,t13,n13,m14,t14,n14,m15,t15,n15,m16,t16,n16,m17,t17,n17,m18,t18,n18,m19,t19,n19,m20,t20,n20,m21,t21,n21,m22,t22,n22,m23,t23,n23,m24,t24,n24,m25,t25,n25,m26,t26,n26,m27,t27,n27,m28,t28,n28,m29,t29,n29,m30,t30,n30,m31,t31,n31,m32,t32,n32,m33,t33,n33,m34,t34,n34,m35,t35,n35,m36,t36,n36,m37,t37,n37,m38,t38,n38,m39,t39,n39,m40,t40,n40,m41,t41,n41,m42,t42,n42,m43,t43,n43,m44,t44,n44,m45,t45,n45,m46,t46,n46,m47,t47,n47,m48,t48,n48,m49,t49,n49,m50,t50,n50,m51,t51,n51,m52,t52,n52,m53,t53,n53,m54,t54,n54,m55,t55,n55,m56,t56,n56,m57,t57,n57,m58,t58,n58,m59,t59,n59,m60,t60,n60,m61,t61,n61,m62,t62,n62,m63,t63,n63,...)   \
    static_assert(# __VA_ARGS__[0] == ',', "xmake_var_list_xxx param count overflow, you need remake this macro by the script which in [macro/xmake_var_list.hpp]");
#define xmake_var_list_check(...)       __xmake_var_list_check__(__VA_ARGS__,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)
