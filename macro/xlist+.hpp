#pragma once
#include"macro/xlist_name.hpp"
#include"macro/xlist_type.hpp"
#include"macro/xlist_args.hpp"
#include"macro/xlist_fields.hpp"

/*
代码生成
================================================================================================
#include<stdio.h>

void make_xlist_xx(int count, int mode){
    const char * select[] ={
        "name",
        "type",
        "args",
        "fields",
    };
    const char * arg[] ={
        "1st",
        "2nd",
        "arg",
        "fields",
    };

    printf("#define __xlist_%s(", select[mode]);
    
    for (int i = 0; i < count; i++){
        printf("a%d,", i);
    }

    printf("...)    \\\n");
    printf("__xheader_%s_ ## a0    \\\n", arg[mode]);
    
    for (int i = 1; i < count; i++){
        printf("__xget_%s_ ## a%-10d\\\n", arg[mode], i);
    }
    
    printf("\n");
    printf("#define xlist_%s(...)   __xlist_%s(__VA_ARGS__", select[mode], select[mode]);
    
    for (int i = 1; i < count; i++){
        printf(",");
    }
    printf(")\n\n");
}

int main(){
    for(int i = 0, count = 64; i < 4; i++){
        make_xlist_xx(count, i);
    }
    return 0;
}
*/
