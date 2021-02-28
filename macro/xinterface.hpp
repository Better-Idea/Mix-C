/*
// 说明：xinterface 组件旨在替换虚函数，简化 GC 结构
// 用法：如下

#define xuser mixc::powerful_cat
#include"macro/xinterface.hpp"
#include"io/tty.hpp"

namespace xuser{
    xinterface(
        xname(isay_hello),
        xfunc(say_hello_to, void(asciis some_one)) // <- 没有多余的逗号
    ); // <- 需要分号

    struct cat{
        void say_hello_to(asciis some_one){
            tty.write_line("Hello! ", some_one, ", this is Cat speaking!");
        }
    };

    void hello(isay_hello const & me){
        me->say_hello_to("leon");
    }

    void test(){
        {
            // 正确
            cat          b;
            isay_hello   ish = b;
            ish.say_hello_to("keyti");

            // 正确
            hello(b);

            // 正确
            hello(cat{});
        }

        {
            // 错误 ========================================================
            // isay_hello 接口需要存活的实例（cat）
            // 而 cat 在赋值给 isay_hello 对象 ish 后就析构了
            // 虽然对于无成员变量的 cat 来说，这样并不会产生错误的输出，
            // 但我们建议不要这么做
            isay_hello ish = cat();
            ish.say_hello_to("System");
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
#define xuser mixc::macro_xinterface::inc
#include"macro/private/word.hpp"
#include"macro/private/xlist.hpp"
#include"macro/xnew.hpp"
#include"memop/signature.hpp"
#include"meta/has_constructor.hpp"
#include"meta_seq/tlist.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_xinterface{
    template<class object_t, uxx length_v>
    static inline voidp s_func_table[length_v];

    struct dummy{ enum { __func_length }; };

    template<class func_list_t>
    struct this_call_pair{
        voidp   this_ptr    = nullptr;
        voidp * func_table  = nullptr;

        template<class object_t>
        constexpr this_call_pair(object_t const & this_ref) : 
            this_ptr(xref this_ref), 
            func_table(s_func_table<object_t, func_list_t::__func_length>){
        }

        constexpr this_call_pair(){};
    };

    using this_call_pairp =  this_call_pair<dummy> *;

    template<
        template<uxx, class ...> class  docker_t, 
        uxx                             i, 
        class                           ret,
        class ...                       args_t,
        class ...                       old_overloadx,
        class ...                       new_overloadx
    >
    inline auto make_flat(inc::tlist<new_overloadx...> result, ret(*)(args_t...), old_overloadx...){
        using new_list = inc::tlist<
            new_overloadx...,
            docker_t<i, ret, args_t...>
        >;

        if constexpr (sizeof...(old_overloadx) == 0){
            return new_list{};
        }
        else{
            return make_flat<docker_t, i + 1>(new_list{}, (old_overloadx)nullptr...);
        }
    }

    template<
        template<class ...> class       docker_t,
        class ...                       overloads_t
    >
    inline auto docker_specialize(inc::tlist<overloads_t...>){
        return docker_t<overloads_t...>{};
    }

    template<
        uxx                             i, 
        template<uxx> class         ... func
    >
    struct functional_assembly{
        template<class object_t>
        constexpr functional_assembly(object_t const & this_ref){}
        constexpr functional_assembly(){}
    private:
        template<class> friend struct this_call_pair;
        enum { __func_length = i };
    };

    template<
        uxx                             i,
        template<uxx> class             func, 
        template<uxx> class         ... funcx
    >
    struct functional_assembly<i, func, funcx...> : 
        func<i>, 
        functional_assembly<i + func<i>::__overload_list::length, funcx...>{

        using base_t = functional_assembly<i + func<i>::__overload_list::length, funcx...>;

        template<class object_t>
        requires(
            inc::has_constructor<func<i>, void(object_t)>
        )
        constexpr functional_assembly(object_t const & this_ref) :
            func<i>(this_ref), base_t(this_ref){
        }
        constexpr functional_assembly(){}
    };

    template<
        template<uxx> class ...         funcx,
        template<uxx> class ...         func_next,
        class ...                       fax
    >
    inline auto make_assembly(
        functional_assembly<0, funcx...>, 
        functional_assembly<0, func_next...>, 
        fax ... list){

        using new_fa = functional_assembly<0, funcx..., func_next...>;

        if constexpr (sizeof...(fax) == 0){
            return new_fa{};
        }
        else{
            return make_assembly(new_fa{}, list...);
        }
    }

    template<class fa, class ... fax>
    using functional_assemblyx      = decltype(
        make_assembly(
            functional_assembly<0>{}, 
            fax{}...,
            fa{}
        )
    );

    template<
        template<uxx, class ...> class  func_docker,
        uxx                             i,
        class                       ... overloads_t
    >
    using overload_list = decltype(
        make_flat<func_docker, i>(inc::tlist<>{}, (overloads_t *)nullptr...)
    );

    template<
        template<class ...> class       docker_t,
        class                           overload_list
    >
    using specialized_docker = decltype(
        docker_specialize<docker_t>(overload_list{})
    );

    template<class target, class constructor>
    concept __itf_hc    = inc::has_constructor<target, constructor>;

    template<class ... type>
    using __itf_tl      = inc::tlist<type...>;

    template<class func>
    using __itf_sg      = inc::signature<func>;
};

template<class __func_list>
using __itf_tcp         = mixc::macro_xinterface::this_call_pair<__func_list>;

using __itf_tcpp        = mixc::macro_xinterface::this_call_pairp;

template<class target, class constructor>
concept __itf_hc        = mixc::macro_xinterface::__itf_hc<target, constructor>;

template<class ... type>
using __itf_tl          = mixc::macro_xinterface::__itf_tl<type...>;

template<class func>
using __itf_sg          = mixc::macro_xinterface::__itf_sg<func>;

template<
    template<uxx, class ...> class  func_docker,
    uxx                             i,
    class ...                       overloads_t
>
using __itf_ol          = mixc::macro_xinterface::overload_list<func_docker, i, overloads_t...>;

template<
    template<class ...> class       docker_t,
    class                           overload_list
>
using __itf_sd          = mixc::macro_xinterface::specialized_docker<docker_t, overload_list>;

template<
    uxx                             i, 
    template<uxx> class         ... func
>
using __itf_fa          = mixc::macro_xinterface::functional_assembly<i, func...>;

template<class fa, class ... fax>
using __itf_fax         = mixc::macro_xinterface::functional_assemblyx<
    // fa::__func_list 会在 functional_assemblyx 中调整到末尾位置以符合继承结构的内存布局
    typename fa::__func_list,
    typename fax::__func_listx...
>;

using __itf_voidp       = voidp;

#define __xitf_name__
#define __xitf_name_name__(name)                name
#define __xitf_name_tmpl__(name,...)            name
#define __xitf_name_spec__(name,...)            name
#define __xitf_name_pubb__(...)
#define __xitf_name_func__(...)
#define __xitf_name_oper__(...)

#define __xitf_namex__
#define __xitf_namex_name__(name)               name
#define __xitf_namex_tmpl__(name,...)           name
#define __xitf_namex_spec__(name,...)           name<__VA_ARGS__>
#define __xitf_namex_pubb__(...)
#define __xitf_namex_func__(...)
#define __xitf_namex_oper__(...)

#define __xitf_pubb__
#define __xitf_pubb_name__(...)
#define __xitf_pubb_tmpl__(...)
#define __xitf_pubb_spec__(...)
#define __xitf_pubb_pubb__(...)                 , __VA_ARGS__
#define __xitf_pubb_func__(...)
#define __xitf_pubb_oper__(...)

#define __xitf_decl_func__
#define __xitf_decl_func_name__(name,...)       using the_t = name;
#define __xitf_decl_func_tmpl__(name,...)       using the_t = name<__VA_ARGS__>;
#define __xitf_decl_func_spec__(name,...)       using the_t = name<__VA_ARGS__>;
#define __xitf_decl_func_pubb__(...)
#define __xitf_decl_func_func__(name,...)                                           \
    template<uxx __i, class __ret, class ... __args>                                \
    struct __lv0_ ## name{                                                          \
        constexpr __lv0_ ## name(){}                                                \
                                                                                    \
        template<class __object>                                                    \
        requires(__itf_sg<__ret(__args...)>::has(& __object::name))                 \
        constexpr __lv0_ ## name(__object const &){                                 \
            struct __closure{                                                       \
                static __ret __shell(__object * __this_ptr, __args ... __list){     \
                    return __this_ptr->name(__list...);                             \
                }                                                                   \
            };                                                                      \
                                                                                    \
            __itf_tcpp(this)->func_table[__i] = __itf_voidp(& __closure::__shell);  \
        }                                                                           \
                                                                                    \
        __ret name(__args ... __list) const {                                       \
            __itf_tcpp __p  = __itf_tcpp(this);                                     \
            using __f       = __ret(*)(__itf_voidp, __args...);                     \
            return __f(__p->func_table[__i])(__p->this_ptr, __list...);             \
        }                                                                           \
    };                                                                              \
                                                                                    \
    template<class ... __overload>                                                  \
    struct __lv1_ ## name : __overload...{                                          \
        template<class __object>                                                    \
        requires(... and __itf_hc<__overload, void(__object)>)                      \
        __lv1_ ## name(__object const & this_ref) :                                 \
            __overload(this_ref)...{                                                \
        }                                                                           \
        __lv1_ ## name(){}                                                          \
                                                                                    \
        using __overload::name...;                                                  \
        using __overload_list = __itf_tl<__overload...>;                            \
    };                                                                              \
                                                                                    \
    template<uxx __i> using __ ## name = __itf_sd<                                  \
        __lv1_ ## name, __itf_ol<                                                   \
            __lv0_ ## name, __i, __VA_ARGS__                                        \
        >                                                                           \
    >;

#define __xitf_decl_func_oper__(...)

#define __xitf_layout_func__
#define __xitf_layout_func_name__(...)
#define __xitf_layout_func_tmpl__(...)
#define __xitf_layout_func_spec__(...)
#define __xitf_layout_func_pubb__(...)
#define __xitf_layout_func_func__(name,...)  , __ ## name
#define __xitf_layout_func_oper__(...)

#define xinterface(...)                                                             \
struct __xlist__(__xitf_namex_, __VA_ARGS__) {                                      \
    __xlist__(__xitf_decl_func_, __VA_ARGS__)                                       \
    using __func_list  = __itf_fa<0 __xlist__(__xitf_layout_func_, __VA_ARGS__)>;   \
    using __func_listx = __itf_fax<the_t __xlist__(__xitf_pubb_,__VA_ARGS__)>;      \
                                                                                    \
    template<class __object>                                                        \
    requires(__itf_hc<__func_listx, void(__object)>)                                \
    constexpr __xlist__(__xitf_name_, __VA_ARGS__)(__object const & __this_ref) :   \
        __pair(__this_ref){                                                         \
                                                                                    \
        if (__pair.func_table[0] == nullptr){                                       \
            xnew (this) __func_listx(__this_ref);                                   \
        }                                                                           \
    }                                                                               \
                                                                                    \
    constexpr __xlist__(__xitf_name_, __VA_ARGS__)(){}                              \
                                                                                    \
    __func_listx const * operator-> () const {                                      \
        return (__func_listx *)this;                                                \
    }                                                                               \
private:                                                                            \
    __itf_tcp<__func_listx> __pair;                                                 \
}

#endif
