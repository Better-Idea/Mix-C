#pragma once
#include"macro/xlink.hpp"
#include"macro/xmake_var_list.hpp"
#include"meta/is_same.hpp"
#include"memop/cast.hpp"

namespace mixc{
    template<int i> struct interface{};
    template<int i> struct place_holder{
        static constexpr int index = i;
    };
}

#define xitf(name)                                                                                  \
using mixc::interface;                                                                              \
using name = interface<__COUNTER__>;                                                                \
template<> struct interface<__COUNTER__ - 1>{                                                       \
private:                                                                                            \
    using __self__ = interface<__COUNTER__ - 2>;                                                    \
    enum { __start__ = __COUNTER__ + 1, };                                                          \
public:                                                                                             \

#define __xitf_item__(index,name,ret,...)                                                           \
    ret name(xmake_var_list_type_name(__VA_ARGS__)){                                                \
        using action = ret(__self__::*)(xmake_var_list_type(__VA_ARGS__));                          \
        auto func = mixc::cast<action>(funcs[index]);                                               \
        if constexpr(mixc::is_same<void, ret>){                                                     \
            (object->*func)(xmake_var_list_name(__VA_ARGS__));                                      \
        }                                                                                           \
        else{                                                                                       \
            return (object->*func)(xmake_var_list_name(__VA_ARGS__));                               \
        }                                                                                           \
    }                                                                                               \
private:                                                                                            \
    xmake_var_list_check(__VA_ARGS__);                                                              \
    template<class type>                                                                            \
    interface(                                                                                      \
        type const & impl,                                                                          \
        void **,                                                                                    \
        mixc::place_holder<index>                                                                   \
    ):  interface(impl, nullptr, mixc::place_holder<index + 1>()){                                  \
        using action = ret(type::*)(xmake_var_list_type(__VA_ARGS__));                              \
        funcs[index] = mixc::cast<void *>(action(& type::name));                                    \
    }                                                                                               \
public:                                                                                             \

#define xitf_item(name,ret,...)  __xitf_item__(xarg(__COUNTER__ - __start__),name,ret,__VA_ARGS__)

#define xitf_end()                                                                                  \
    template<class type>                                                                            \
    interface(type const & impl) :                                                                  \
        interface(impl, funcs = func_list<type>, mixc::place_holder<0>()){                          \
    }                                                                                               \
private:                                                                                            \
    template<class type, int i>                                                                     \
    interface(type const & impl, void ** current_func, place_holder<i>) {}                          \
    interface * object;                                                                             \
    void **     funcs;                                                                              \
    template<class type>                                                                            \
    inline static void * func_list[__COUNTER__ - __start__];                                        \
};
