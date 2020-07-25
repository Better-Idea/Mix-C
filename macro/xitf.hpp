/*
// 注意：===============================================================
// xitf 内部使用了显式的 this_call 调用，需要留意可能的 ABI 问题
// =====================================================================

// 说明：xitf 组件旨在替换虚函数，简化 GC 结构
// 用法：如下

#include<stdio.h>
#include"macro/xitf.hpp"

xitf(
    xname(can_fly),
    xfunc(fly, 
        xret(void),
        xarg(arg, int)
    )
)

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
#include"configure.hpp"
#include"macro/private/xlist.hpp"
#include"macro/private/word.hpp"
#include"memop/addressof.hpp"
#include"memop/signature.hpp"
#include"meta/has_cast.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/tappend.hpp"
#pragma pop_macro("xuser")

#define __xitf_ret__
#define __xitf_ret_ret__(...)               __VA_ARGS__
#define __xitf_ret_arg__(...)

#define __xitf_arg0__
#define __xitf_arg0_ret__(name,...)
#define __xitf_arg0_arg__(name,...)         __VA_ARGS__

#define __xitf_argx__
#define __xitf_argx_ret__(name,...)
#define __xitf_argx_arg__(name,...)         , __VA_ARGS__

#define __xitf_layout_arg0__
#define __xitf_layout_arg0_ret__(name,...)
#define __xitf_layout_arg0_arg__(name,...)  __VA_ARGS__ name

#define __xitf_layout_argx__
#define __xitf_layout_argx_ret__(name,...)
#define __xitf_layout_argx_arg__(name,...)  , __VA_ARGS__ name

#define __xitf_set_argx__
#define __xitf_set_argx_ret__(name,...)
#define __xitf_set_argx_arg__(name,...)     , name

// 结构名
#define __xitf_name__
#define __xitf_name_name__(name)            name
#define __xitf_name_tmpl__(name,...)        name
#define __xitf_name_spec__(name,...)        name<__VA_ARGS__>
#define __xitf_name_pubb__(...)
#define __xitf_name_func__(...)

// 构造函数名
#define __xitf_cons__
#define __xitf_cons_name__(name)            name
#define __xitf_cons_tmpl__(name,...)        name
#define __xitf_cons_spec__(name,...)        name
#define __xitf_cons_pubb__(...)
#define __xitf_cons_func__(...)

#define __xitf_check__
#define __xitf_check_name__(...)
#define __xitf_check_tmpl__(...)
#define __xitf_check_spec__(...)
#define __xitf_check_func__(name,...)                                   \
and ::mixc::memop_signature::signature<                                 \
    __xlista__(__xitf_ret_, __VA_ARGS__)(                               \
    __xlista3__(__xitf_arg0_, __xitf_arg0_, __xitf_argx_, ##__VA_ARGS__)\
)>::has(& __object::name)

#define __xitf_registration__
#define __xitf_registration_name__(...)
#define __xitf_registration_tmpl__(...)
#define __xitf_registration_spec__(...)
#define __xitf_registration_func__(name,...)                            \
func_list[i++] = ::mixc::memop_signature::signature<                    \
    __xlista__(__xitf_ret_, __VA_ARGS__)(                               \
    __xlista3__(__xitf_arg0_, __xitf_arg0_, __xitf_argx_, ##__VA_ARGS__)\
)>::check(& __object::name);

#define __xitf_invoke__
#define __xitf_invoke_name__(...)
#define __xitf_invoke_tmpl__(...)
#define __xitf_invoke_spec__(...)
#define __xitf_invoke_func__(name,...)                                  \
__xlista__(__xitf_ret_, __VA_ARGS__) name(                              \
__xlista3__(                                                            \
    __xitf_layout_arg0_,                                                \
    __xitf_layout_arg0_,                                                \
    __xitf_layout_argx_, ##__VA_ARGS__)                                 \
) const {                                                               \
    auto self = ::mixc::macro_xitf::itf_datap(this);                    \
    return ::mixc::memop_signature::signature<                          \
        __xlista__(__xitf_ret_, ## __VA_ARGS__)(                        \
        __xlista3__(                                                    \
            __xitf_arg0_,                                               \
            __xitf_arg0_,                                               \
            __xitf_argx_, ##__VA_ARGS__                                 \
        )                                                               \
    )>::call(                                                           \
        self->object,                                                   \
        self->func_list[__COUNTER__ - __index_begin]                    \
        __xlista__(__xitf_set_argx_, ##__VA_ARGS__)                     \
    );                                                                  \
}

#define __xitf_func_cnt__
#define __xitf_func_cnt_name__(...)
#define __xitf_func_cnt_tmpl__(...)
#define __xitf_func_cnt_spec__(...)
#define __xitf_func_cnt_func__(...)         +1

#define xitf(...)                                                           \
struct                                                                      \
__xlist__(__xitf_name_, __VA_ARGS__) {                                      \
protected:                                                                  \
    friend struct ::mixc::macro_xitf::helper;                               \
    template<class __object>                                                \
    requires(true/*dummy*/ __xlist__(__xitf_check_, ##__VA_ARGS__))         \
    static constexpr bool __xitf_check() {                                  \
        return true;                                                        \
    }                                                                       \
                                                                            \
    template<class __object>                                                \
    static constexpr bool __xitf_check() {                                  \
        return false;                                                       \
    }                                                                       \
                                                                            \
    enum{                                                                   \
        __invoke_count = 0 __xlist__(__xitf_func_cnt_, ##__VA_ARGS__),      \
        __index_begin  = __COUNTER__ + 2 + __invoke_count,                  \
    };                                                                      \
public:                                                                     \
    template<uxx __invoke_offset>                                           \
    struct core{                                                            \
    protected:                                                              \
        friend struct ::mixc::macro_xitf::helper;                           \
        enum{                                                               \
            __invoke_count = 0 __xlist__(__xitf_func_cnt_, ##__VA_ARGS__),  \
            __index_begin  = __COUNTER__ + 1 - __invoke_offset,             \
        };                                                                  \
                                                                            \
    public:                                                                 \
        core(){}                                                            \
                                                                            \
        template<class __object>                                            \
        core(voidp ** func_listp, __object const &) {                       \
            voidp * func_list = func_listp[0];                              \
            uxx     i = 0;                                                  \
            __xlist__(__xitf_registration_, __VA_ARGS__)                    \
            func_listp[0] += i;                                             \
        }                                                                   \
    public:                                                                 \
        __xlist__(__xitf_invoke_, ##__VA_ARGS__)                            \
    };                                                                      \
                                                                            \
protected:                                                                  \
    ::mixc::macro_xitf::itf_data __data;                                    \
public:                                                                     \
    template<class __object>                                                \
    __xlist__(__xitf_cons_, ##__VA_ARGS__)(__object const & object){        \
        using meta = core<0>;                                               \
        __data.object    = xref object;                                     \
        __data.func_list =                                                  \
            ::mixc::macro_xitf::func_list<__object, __invoke_count>;        \
        new (this) meta(xref __data.func_list, object);                     \
        __data.func_list =                                                  \
            ::mixc::macro_xitf::func_list<__object, __invoke_count>;        \
    }                                                                       \
    __xlist__(__xitf_invoke_, ##__VA_ARGS__)                                \
};

namespace mixc::macro_xitf{
    typedef struct itf_data{
        voidp   object      = nullptr;
        voidp * func_list   = nullptr;
    } * itf_datap;

    template<class object, uxx invoke_count>
    static inline voidp func_list[invoke_count];

    template<uxx invoke_count, class ... args>
    struct meta : args...{
    private:
        itf_data data;
    public:
        meta(){}

        template<class object>
        meta(object const & obj) : 
            data{ xref obj, func_list<object, invoke_count> }, args(xref data.func_list, obj)...  {
            data.func_list = func_list<object, invoke_count>;
        }
    };

    struct helper {
        template<class object, uxx invoke_count>
        static inline voidp func_list[invoke_count];

        template<uxx offset, class base_list, template<uxx> class first, template<uxx> class ... rest>
        static auto merge(base_list) {
            using item                = first<offset>;
            using new_list            = typename inc::tappend<base_list, item>::new_list;
            constexpr uxx next_offset = offset + item::__invoke_count;
            return merge<next_offset, new_list, rest...>(new_list());
        }

        template<uxx invoke_count, class tlist>
        static auto merge(tlist) {
            return calc<invoke_count>(tlist());
        }

        template<uxx invoke_count, class ... args>
        static auto calc(inc::tlist<args...>) {
            return meta<invoke_count, args...>();
        }
    };

    template<class ... args>
    using hub_itf = decltype(
        helper::merge<0, inc::tlist<>, args::template core...>(inc::tlist<>())
    );
}

#endif

namespace xuser::inc {
    using ::mixc::macro_xitf::hub_itf;
}
