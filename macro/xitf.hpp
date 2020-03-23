/*
说明：xitf 组件旨在替换虚函数，简化 GC 结构
用法：如下

#include<stdio.h>
#include"macro/xitf.hpp"

xitf(can_fly)
    xitf_item(fly, void,
        xnt(arg, int)
    );
xitf_end();

struct bird{
    void fly(int a){
        printf("i'm bird, i can fly %d!\n", a);
    }
};

void fly(can_fly ufo){
    ufo.fly(2020);
}

int main(){
    // 正确
    bird b;
    can_fly cf = b;
    cf.fly(2020);

    // 正确
    fly(b);

    // 正确
    fly(bird());
    
    // 错误 ========================================================
    // xitf 接口（can_fly）需要存活的实例（bird）
    // 而 bird 在赋值给 can_fly 对象 cf 后就析构了
    // 虽然对于无成员变量的 bird 来说，这样并不会产生错误的输出，
    // 但我们建议不要这么做
    can_fly cf = bird();
    cf.flay(2020);
    return 0;
}
*/

#ifndef xpack_macro_xitf
#define xpack_macro_xitf
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xitf
        #include"dumb/place_holder.hpp"
        #include"macro/private/xprefix.hpp"
        #include"macro/xvaargs.hpp"
        #include"macro/xlist+.hpp"
        #include"memop/cast.hpp"
        #include"meta/is_same.hpp"
    #pragma pop_macro("xuser")

    #define xitf(name,...)                                                                              \
    struct name{                                                                                        \
    private:                                                                                            \
        using __self__ = name __xprefix_keep_tmpl_ ## __VA_ARGS__;                                      \
        __self__ *  __object;                                                                           \
        void    **  __func_list;                                                                        \
        enum { __start__ = __COUNTER__ + 1, };                                                          \
    public:                                                                                             \
        template<class __type__>                                                                        \
        name(__type__ const & impl) {                                                                   \
            __func_list = __func_list__<__type__>;                                                      \
            __build__(impl, mixc::dumb_place_holder::place_holder<0>());                                \
        }                                                                                               \

    #define __xitf_item__(index,name,ret,...)                                                           \
        ret name(xlist_args(__VA_ARGS__)) const {                                                              \
            using action = ret(__self__::*)(xlist_type(__VA_ARGS__));                                   \
            auto func = mixc::memop_cast::cast<action>(__func_list[index]);                             \
            if constexpr(mixc::meta_is_same::is_same<void, ret>){                                       \
                (__object->*func)(xlist_name(__VA_ARGS__));                                             \
            }                                                                                           \
            else{                                                                                       \
                return (__object->*func)(xlist_name(__VA_ARGS__));                                      \
            }                                                                                           \
        }                                                                                               \
    private:                                                                                            \
        template<class __type__>                                                                        \
        void __build__(__type__ const & impl, mixc::dumb_place_holder::place_holder<index>){            \
            __build__(impl, mixc::dumb_place_holder::place_holder<index + 1>());                        \
            using action = ret(__type__::*)(xlist_type(__VA_ARGS__));                                   \
            __func_list[index] = mixc::memop_cast::cast<void *>(action(& __type__::name));              \
        }                                                                                               \
    public:                                                                                             \

    #define xitf_item(name,ret,...)                                                                     \
        __xitf_item__(xvaargs(__COUNTER__ - __start__),name,ret,__VA_ARGS__)

    #define xitf_end()                                                                                  \
    private:                                                                                            \
        template<class __type__>                                                                        \
        void __build__(                                                                                 \
            __type__ const & impl,                                                                      \
            mixc::dumb_place_holder::place_holder<__COUNTER__ - __start__>) {}                          \
        template<class __type__>                                                                        \
        inline static void * __func_list__[__COUNTER__ - __start__ - 1];                                \
    }

#endif
