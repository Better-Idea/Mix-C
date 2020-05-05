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

xitf(can_fly
    xitem(fly, void, int)
);

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
    cf.fly(2020);
    return 0;
}
*/

#ifndef xpack_macro_xitf
#define xpack_macro_xitf
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xitf
        #include"macro/private/xlist.hpp"
        #include"memop/addressof.hpp"
        #include"memop/signature.hpp"
        #include"meta/has_cast.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xitf{
        struct __invoke_table_t{
            voidp   object  = nullptr;
            voidp * funcs   = nullptr;
        };

        template<int __invoke_id, class __ret, class ... __args>
        struct __invoke_t : private __invoke_table_t{
            __ret operator()(__args ... args) const {
                return inc::signature<__ret, __args...>::call(
                    object, 
                    funcs[__invoke_id], 
                    args...
                );
            }
        };

        #define __xprefix_check_invoke_
        #define __xprefix_check_invoke_tmpl__(...)
        #define __xprefix_check_invoke_cast__(...)
        #define __xprefix_check_invoke_item__(func_name,ret_type,...)                               \
            inner::signature<ret_type, ## __VA_ARGS__>::check(& impl.func_name);

        #define __xprefix_map_invoke_
        #define __xprefix_map_invoke_tmpl__(...)
        #define __xprefix_map_invoke_cast__(...)                                                    \
            __table->funcs[__COUNTER__ - __start] =                                                 \
            inner::signature<__VA_ARGS__>::check(& impl.operator __VA_ARGS__);
        #define __xprefix_map_invoke_item__(func_name,ret_type,...)                                 \
            __table->funcs[__COUNTER__ - __start] =                                                 \
            inner::signature<ret_type, ## __VA_ARGS__>::check(& impl.func_name);
        
        #define __xprefix_emplace_invoke_
        #define __xprefix_emplace_invoke_tmpl__(...)
        #define __xprefix_emplace_invoke_cast__(...)                                                \
            operator __VA_ARGS__ () const {                                                         \
                auto & invoke = *(inner::__invoke_t<__COUNTER__ - 1 - __end, __VA_ARGS__> *)this;   \
                return invoke();                                                                    \
            }
        #define __xprefix_emplace_invoke_item__(func_name,ret_type,...)                             \
            const inner::__invoke_t<__COUNTER__ - 1 - __end, ret_type, ## __VA_ARGS__> func_name;

        #define __xprefix_requires_invoke_
        #define __xprefix_requires_invoke_tmpl__(...)
        #define __xprefix_requires_invoke_item__(...)
        #define __xprefix_requires_invoke_cast__(...)                                               \
            and inner::has_cast<__VA_ARGS__, object>

        #define __xprefix_tmpl_invoke_
        #define __xprefix_tmpl_invoke_item__(...)
        #define __xprefix_tmpl_invoke_cast__(...)
        #define __xprefix_tmpl_invoke_tmpl__(...)  template<__VA_ARGS__>

        #define xcast(...)      cast__(__VA_ARGS__)
        #define xitem(...)      item__(__VA_ARGS__)
        #define xtmpl(...)      tmpl__(__VA_ARGS__)

        #define xitf(name,...)                                                                      \
        namespace inner{                                                                            \
            using namespace ::mixc::macro_xitf;                                                     \
            using namespace ::mixc::macro_xitf::inc;                                                \
        }                                                                                           \
        template<class object>                                                                      \
        concept name ## _t = requires(object impl){                                                 \
            __xlist__(check_invoke_, check_invoke_, __VA_ARGS__)                                    \
        };                                                                                          \
        __xlist__(tmpl_invoke_, tmpl_invoke_, __VA_ARGS__) union name{                              \
        private:                                                                                    \
            enum{ __start = __COUNTER__ + 1 };                                                      \
        public:                                                                                     \
            name(){}                                                                                \
            template<name ## _t object> requires(                                                   \
                true __xlist__(requires_invoke_, requires_invoke_, __VA_ARGS__)                     \
            )                                                                                       \
            name(object const & impl){                                                              \
                inner::__invoke_table_t * __table = (inner::__invoke_table_t *)this;                \
                __table->object = inner::addressof(impl);                                           \
                __table->funcs  = __func_list<object>;                                              \
                __xlist__(map_invoke_, map_invoke_, __VA_ARGS__)                                    \
            }                                                                                       \
        private:                                                                                    \
            enum { __end = __COUNTER__ };                                                           \
            template<class object>                                                                  \
            static inline voidp __func_list[__end - __start];                                       \
        public:                                                                                     \
            __xlist__(emplace_invoke_, emplace_invoke_, __VA_ARGS__);                               \
        }
    }

#endif
