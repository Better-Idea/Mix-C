/*
// 说明：xinterface 组件旨在替换虚函数，简化 GC 结构
// 用法：如下

#define xuser mixc::powerful_cat
#include"macro/xinterface.hpp"
#include"io/tty.hpp"

namespace xuser{
    xinterface(
        xname(can_say_hello),
        xfunc(say_hello_to, void(asciis some_one)) // <- 没有多余的逗号
    ); // <- 需要分号

    struct cat{
        void say_hello_to(asciis some_one){
            tty.write_line("Hello! ", some_one, ", this is Cat speaking!");
        }
    };

    void hello(can_say_hello const & me){
        me.say_hello_to("leon");
    }

    void test(){
        {
            // 正确
            cat             b;
            can_say_hello   csh = b;
            csh.say_hello_to("keyti");

            // 正确
            hello(b);

            // 正确
            hello(cat{});
        }

        {
            // 错误 ========================================================
            // can_say_hello 接口需要存活的实例（cat）
            // 而 cat 在赋值给 can_say_hello 对象 csh 后就析构了
            // 虽然对于无成员变量的 cat 来说，这样并不会产生错误的输出，
            // 但我们建议不要这么做
            can_say_hello csh = cat();
            csh.say_hello_to("System");
        }
    }
}

int main(){
    xuser::test();
    return 0;
}
*/

#ifndef xpack_macro_xinterface
#define xpack_macro_xinterface
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xinterface
#include"dumb/place_holder.hpp"
#include"macro/private/word.hpp"
#include"macro/private/xlist.hpp"
#include"macro/xnew.hpp"
#include"memop/signature.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_xinterface{
    template<uxx offset_this, class func_t> struct functor;

    template<uxx offset_this, class ret_t, class ... args>
    struct functor<offset_this, ret_t(args...)>{
    private:
        voidp __this_ptr() const {
            return *(voidp *)(u08p(this) - offset_this); // 偏移字节数
        }

        template<class object_t>
        static ret_t meta_form(object_t * this_ptr, args ... arg_list) {
            return this_ptr->operator()(arg_list...);
        }

        voidp __func    = nullptr;
    public:
        constexpr functor(){}

        template<class object_t>
        constexpr functor(object_t const & object):
            __func(voidp(& meta_form<object_t>)){
        }

        ret_t operator()(args const & ... params) const {
            return ((ret_t(*)(voidp, args...))__func)(__this_ptr(), params...);
        }
    };

    template<class func_t> 
    struct functor_closure{
        template<uxx offset_this>
        using __core = functor<offset_this, func_t>;
    };

    struct this_ptr{ 
        template<uxx>
        struct __core{
            template<class object_t>
            __core(object_t const & object):
                self(voidp(& object)){
            }
        private:
            voidp self;
        };
    };

    template<uxx offset, class ... args> 
    struct interface_hub_core{
        template<class object_t>
        interface_hub_core(object_t const & object) {}
    };

    template<uxx offset, class a0, class ... args>
    struct interface_hub_core<offset, a0, args...> : 
        a0::__core<offset>, 
        interface_hub_core<
            offset + sizeof(typename a0::__core<offset>), 
            args...
        >{

        template<class object_t>
        interface_hub_core(object_t const & object) : 
            a0::__core<offset>(object), 
            interface_hub_core<
                offset + sizeof(typename a0::__core<offset>), 
                args...
            >(object){
        }
    };

    template<class ... args>
    using interface_hub = interface_hub_core<0, args...>;

    template<class target_interface, class object_t>
    concept is_match_interface = requires(target_interface * ptr, object_t const & type){
        xnew (ptr) target_interface(type);
    };
};

#define __xitf_name__
#define __xitf_name_name__(name)                name
#define __xitf_name_tmpl__(name,...)            name
#define __xitf_name_spec__(name,...)            name<__VA_ARGS__>
#define __xitf_name_pubb__(...)
#define __xitf_name_func__(...)
#define __xitf_name_oper__(...)

#define __xitf_pubb__
#define __xitf_pubb_name__(...)
#define __xitf_pubb_tmpl__(...)
#define __xitf_pubb_spec__(...)
#define __xitf_pubb_pubb__(...)                 , __VA_ARGS__
#define __xitf_pubb_func__(...)
#define __xitf_pubb_oper__(...)                 , mixc::macro_xinterface::functor_closure<__VA_ARGS__>

#define __xitf_set_func__
#define __xitf_set_func_name__(...)                                             \
    enum : uxx { __meta_form_cnt_begin = __COUNTER__ + 1 };
#define __xitf_set_func_tmpl__(...)
#define __xitf_set_func_spec__(...)
#define __xitf_set_func_pubb__(...)
#define __xitf_set_func_func__(name,...)        {                                                           \
    constexpr auto i = __COUNTER__ - __meta_form_cnt_begin;                                                 \
    __func_list(i) = __meta_form(__ph<i>{}, object, __sg<__VA_ARGS__>::fetch(& __object::name));            \
}
#define __xitf_set_func_oper__(...)

#define __xitf_decl_func__
#define __xitf_decl_func_name__(...)
#define __xitf_decl_func_tmpl__(...)
#define __xitf_decl_func_spec__(...)
#define __xitf_decl_func_pubb__(...)
#define __xitf_decl_func_func__(name,...)       __fc<__COUNTER__, __VA_ARGS__> name;
#define __xitf_decl_func_oper__(...)

#define __xitf_cast_func__
#define __xitf_cast_func_name__(...)                                            \
    enum : uxx { __meta_form_cnt_begin = __COUNTER__ + 1 };
#define __xitf_cast_func_tmpl__(...)
#define __xitf_cast_func_spec__(...)
#define __xitf_cast_func_pubb__(...)
#define __xitf_cast_func_func__(name,...)                                       \
    template<                                                                   \
        class __object,                                                         \
        class __ret,                                                            \
        class __owner,                                                          \
        class ... __args                                                        \
    >                                                                           \
    static voidp __meta_form(                                                   \
        __ph<__COUNTER__ - __meta_form_cnt_begin>,                              \
        __object const &,                                                       \
        __ret(__owner::*)(__args...)){                                          \
        struct __closure{                                                       \
            static voidp __meta(__object * this_ptr, __args ... list){          \
                return this_ptr->name(list...);                                 \
            }                                                                   \
        };                                                                      \
        return voidp(& __closure::__meta);                                      \
    }

#define __xitf_cast_func_oper__(...)

#define __xitf_core__(name,base,...)                                            \
private:                                                                        \
    enum : uxx { __cnt_begin = __COUNTER__ + 1 };                               \
                                                                                \
    template<uxx __i, class __func>                                             \
    using __fc = ::mixc::macro_xinterface::                                     \
        functor<(__i - __cnt_begin) * sizeof(voidp) + __global_offset, __func>; \
public:                                                                         \
    template<class __object>                                                    \
    constexpr name(__object const & object) base {                              \
        /* 给 functor 赋值（指向指定的成员函数） */                             \
        __xlist__(__xitf_set_func_, __VA_ARGS__)                                \
    }                                                                           \
                                                                                \
    /* 定义 functor */                                                          \
    __xlist__(__xitf_decl_func_, __VA_ARGS__)

#define xinterface(...)                                                         \
struct __xlist__(__xitf_name_, __VA_ARGS__) :                                   \
    ::mixc::macro_xinterface::interface_hub<                                    \
        ::mixc::macro_xinterface::this_ptr                                      \
        __xlist__(__xitf_pubb_, __VA_ARGS__)                                    \
    > {                                                                         \
                                                                                \
    using __base_hub =                                                          \
        ::mixc::macro_xinterface::interface_hub<                                \
            ::mixc::macro_xinterface::this_ptr                                  \
            __xlist__(__xitf_pubb_, __VA_ARGS__)                                \
        >;                                                                      \
                                                                                \
private:                                                                        \
    template<uxx __i>                                                           \
    using __ph = ::mixc::dumb_place_holder::place_holder<__i>;                  \
                                                                                \
    template<class __func>                                                      \
    using __sg = ::mixc::memop_signature::signature<__func>;                    \
    __xlist__(__xitf_cast_func_, __VA_ARGS__)                                   \
public:                                                                         \
    template<uxx __global_offset>                                               \
    struct __core{                                                              \
        __xitf_core__(__core, , __VA_ARGS__)                                    \
    private:                                                                    \
        voidp & __func_list(uxx i) const {                                      \
            return ((voidp *)this)[i];                                          \
        }                                                                       \
    };                                                                          \
private:                                                                        \
    enum : uxx { __global_offset = sizeof(__base_hub) };                        \
                                                                                \
    voidp & __func_list(uxx i) const {                                          \
        return ((voidp *)(u08p(this) + __global_offset))[i];                    \
    }                                                                           \
                                                                                \
    __xitf_core__(                                                              \
        __xlist__(__xitf_name_, __VA_ARGS__), :                                 \
        __base_hub(object),                                                     \
        __VA_ARGS__                                                             \
    )                                                                           \
}

#endif
