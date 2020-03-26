/*
注意：===============================================================
xitf 内部使用了显式的 this_call 调用，需要留意可能的 ABI 问题
=====================================================================
*/

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
        #include"macro/xlist+.hpp"
        #include"macro/xvaargs.hpp"
        #include"memop/addressof.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xitf{
        template<class ret, class ... args>
        struct signature{
            #define xgen(...)                                                           \
                template<class object>                                                  \
                static auto check(ret (object::* this_call)(args...) __VA_ARGS__){      \
                    union {                                                             \
                        void *              result;                                     \
                        decltype(this_call) mem;                                        \
                    };                                                                  \
                    mem = this_call;                                                    \
                    return result;                                                      \
                }
            xgen()
            xgen(const)
            #undef xgen

            static auto call(void * self, void * this_call, args ... list){
                union {
                    ret (* result)(void *, args...);
                    void * mem;
                };
                mem = this_call;
                return result(self, list...);
            }
        };
    }

    #define xitf(name,...)                                                                              \
    struct name{                                                                                        \
    private:                                                                                            \
        void  *   __object;                                                                             \
        void  **  __func_list;                                                                          \
        enum { __start = __COUNTER__ + 1, };                                                            \
    public:                                                                                             \
        template<class __type__>                                                                        \
        name(__type__ const & impl) {                                                                   \
            __func_list = __func_list__<__type__>;                                                      \
            __object    = mixc::memop_addressof::addressof(impl);                                       \
            __build(impl, mixc::dumb_place_holder::place_holder<0>());                                  \
        }                                                                                               \

    #define __xitf_item__(index,name,ret,...)                                                           \
        ret name(xlist_args(__VA_ARGS__)) const {                                                       \
            return mixc::macro_xitf::signature<xlist_type(xnt(__ret, ret), __VA_ARGS__)>::call(         \
                __object,                                                                               \
                xlist_name(xnt(__func_list[index], void *), __VA_ARGS__)                                \
            );                                                                                          \
        }                                                                                               \
    private:                                                                                            \
        template<class __type__>                                                                        \
        void __build(__type__ const & impl, mixc::dumb_place_holder::place_holder<index>){              \
            __func_list[index] = mixc::macro_xitf::signature                                            \
                <xlist_type(xnt(__ret, ret), __VA_ARGS__)>::check(& __type__::name);                    \
            __build(impl, mixc::dumb_place_holder::place_holder<index + 1>());                          \
        }                                                                                               \
    public:

    #define xitf_item(name,ret,...)                                                                     \
        __xitf_item__(xvaargs(__COUNTER__ - __start),name,ret,__VA_ARGS__)

    #define xitf_end()                                                                                  \
    private:                                                                                            \
        template<class __type__>                                                                        \
        void __build(                                                                                   \
            __type__ const & impl,                                                                      \
            mixc::dumb_place_holder::place_holder<__COUNTER__ - __start>) {}                            \
        template<class __type__>                                                                        \
        inline static void * __func_list__[__COUNTER__ - __start - 1];                                  \
    }

#endif
