/* 注意：
 * 在使用 xpubget_pubsetx 构建属性范式时，请勿使用 __COUNTER__ 
 * 在使用 xstruct 宏时不要出现多余的逗号，像这样
 * xstruct(
 *     xname(foo),
 *     xprif(bar, type), <- 多余的逗号
 * )
 */

#ifndef xpack_macro_xstruct
#define xpack_macro_xstruct
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xstruct
#include"dumb/place_holder.hpp"
#include"macro/private/xlist.hpp"
#include"macro/private/word.hpp"
#include"macro/xdummy.hpp"
#include"macro/xlink.hpp"
#include"meta/remove_ref.hpp"
#include"meta_seq/tlist.hpp"
#include"meta_seq/vlist.hpp"
#include"meta_seq/vmerge.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_xtypeid{
    template<class type, class dummy> struct __typeid;
}

namespace mixc::gc_private_tuple{
    template<class root_t> struct tuple;
}

namespace mixc::macro_xtypeid{
    template<class type, class dummy> struct __typeid;
}

namespace mixc::macro_xstruct{
    using namespace inc;

    template<class self, class ... list>
    struct data_field_collector{
    private:
        template<class a0, class ... args, auto ... result>
        static auto merge(vlist<result...>){
            typename vmerge<vlist<result...>, a0>::new_list items;

            if constexpr (sizeof...(args) == 0){
                return items;
            }
            else{
                return merge<args...>(items);
            }
        }
    public:
        using member_list = decltype(merge<
                typename self::member_list_partial,
                typename list::member_list...
            >(vlist<>())
        );
    };

    template<class ignore, class ... rest>
    using base_list = tlist<rest...>;

    template<auto ignore, auto ... rest>
    using member_list_partial = vlist<rest...>;

    template<class type>
    struct fake{ type * item = nullptr; };
}

template<uxx index>
using __dph = mixc::dumb_place_holder::place_holder<index, true>;

template<class self, class ... list>
using __dfc = typename mixc::macro_xstruct::data_field_collector<self, list...>::member_list;

template<class ignore, class ... rest>
using __bl  = mixc::macro_xstruct::base_list<ignore, rest...>;

template<auto ignore, auto ... rest>
using __mlp = mixc::macro_xstruct::member_list_partial<ignore, rest...>;

template<class type>
using __fak = mixc::macro_xstruct::fake<type>;

template<class type>
using __rr  = ::mixc::meta_remove_ref::remove_ref<type>;

static inline uxx __class_id = 0x80000000;

#define __ignore__(...)

// 结构名
#define __xstruct__
#define __xstruct_name__(...)               __VA_ARGS__
#define __xstruct_tmpl__(name,...)          name
#define __xstruct_spec__(name,...)          name<__VA_ARGS__>
#define __xstruct_pubb__(...)
#define __xstruct_prob__(...)
#define __xstruct_prib__(...)
#define __xstruct_pubf__(...)
#define __xstruct_prof__(...)
#define __xstruct_prif__(...)
#define __xstruct_asso__(...)

// 基类
#define __xexpand__
#define __xexpand_name__(...)
#define __xexpand_tmpl__(...)
#define __xexpand_spec__(...)
#define __xexpand_pubb__(...)               , public      __VA_ARGS__
#define __xexpand_prob__(...)               , protected   __VA_ARGS__
#define __xexpand_prib__(...)               , private     __VA_ARGS__
#define __xexpand_pubf__(...)
#define __xexpand_prof__(...)
#define __xexpand_prif__(...)
#define __xexpand_asso__(...)

// 基类
#define __xexpand_first__
#define __xexpand_first_name__(...)
#define __xexpand_first_tmpl__(...)
#define __xexpand_first_spec__(...)
#define __xexpand_first_pubb__(...)         : public      __VA_ARGS__ 
#define __xexpand_first_prob__(...)         : protected   __VA_ARGS__ 
#define __xexpand_first_prib__(...)         : private     __VA_ARGS__ 
#define __xexpand_first_pubf__(...)
#define __xexpand_first_prof__(...)
#define __xexpand_first_prif__(...)
#define __xexpand_first_asso__(...)

// the_t 别名
#define __xthe__
#define __xthe_name__(...)                  __VA_ARGS__
#define __xthe_tmpl__(name,...)             name<__VA_ARGS__>
#define __xthe_spec__(name,...)             name<__VA_ARGS__>
#define __xthe_pubb__(...)
#define __xthe_prob__(...)
#define __xthe_prib__(...)
#define __xthe_pubf__(...)
#define __xthe_prof__(...)
#define __xthe_prif__(...)
#define __xthe_asso__(...)

// base_list 基类清单
#define __xbase__
#define __xbase_name__(...)
#define __xbase_tmpl__(...)
#define __xbase_spec__(...)
#define __xbase_pubb__(...)                 , __VA_ARGS__
#define __xbase_prob__(...)                 , __VA_ARGS__
#define __xbase_prib__(...)                 , __VA_ARGS__
#define __xbase_pubf__(...)
#define __xbase_prof__(...)
#define __xbase_prif__(...)
#define __xbase_asso__(...)

// member_list 所有成员清单
#define __xplaced__
#define __xplaced_name__(...)               __VA_ARGS__
#define __xplaced_tmpl__(name,...)          name<__VA_ARGS__>
#define __xplaced_spec__(name,...)          name<__VA_ARGS__>
#define __xplaced_pubb__(...)               , __VA_ARGS__
#define __xplaced_prob__(...)               , __VA_ARGS__
#define __xplaced_prib__(...)               , __VA_ARGS__
#define __xplaced_pubf__(...)
#define __xplaced_prof__(...)
#define __xplaced_prif__(...)
#define __xplaced_asso__(...)

// 字段
#define __xfield__
#define __xfield_name__(...)
#define __xfield_tmpl__(...)
#define __xfield_spec__(...)
#define __xfield_pubb__(...)
#define __xfield_prob__(...)
#define __xfield_prib__(...)
#define __xfield_pubf__(name,...)           public:     __VA_ARGS__ name;
#define __xfield_prof__(name,...)           protected:  __VA_ARGS__ name;
#define __xfield_prif__(name,...)           private:    __VA_ARGS__ name;
#define __xfield_asso__(...)

// 非基类成员字段指针
#define __xmlist__
#define __xmlist_name__(...)
#define __xmlist_tmpl__(...)
#define __xmlist_spec__(...)
#define __xmlist_pubb__(...)
#define __xmlist_prob__(...)
#define __xmlist_prib__(...)
#define __xmlist_pubf__(name,...)           , & the_t::name
#define __xmlist_prof__(name,...)           , & the_t::name
#define __xmlist_prif__(name,...)           , & the_t::name
#define __xmlist_asso__(...)                , & __fak<__VA_ARGS__>::item

#define __xtype__
#define __xtype_name__(...)                 #__VA_ARGS__
#define __xtype_tmpl__(name,...)            #name
#define __xtype_spec__(name,...)            #name
#define __xtype_pubb__(...)
#define __xtype_prob__(...)
#define __xtype_prib__(...)
#define __xtype_pubf__(...)
#define __xtype_prof__(...)
#define __xtype_prif__(...)
#define __xtype_asso__(...)

#define __xitem__
#define __xitem_name__(...)
#define __xitem_tmpl__(...)
#define __xitem_spec__(...)
#define __xitem_pubb__(...)
#define __xitem_prob__(...)
#define __xitem_prib__(...)
#define __xitem_pubf__(name,...)            #name,
#define __xitem_prof__(name,...)            #name,
#define __xitem_prif__(name,...)            #name,
#define __xitem_asso__(...)                 "",


#define xstruct(...)                                                            \
struct __xlist__(__xstruct_, __VA_ARGS__)                                       \
       __xlist3__(__xexpand_first_, __xexpand_first_, __xexpand_, __VA_ARGS__) {\
    using the_t = __xlist__(__xthe_, __VA_ARGS__);                              \
        __xlist__(__xfield_, __VA_ARGS__)                                       \
    private:                                                                    \
        enum{ __start = __COUNTER__ + 1 };                                      \
        static constexpr auto __my_name =                                       \
            __xlist__(__xtype_, __VA_ARGS__);                                   \
        static inline uxx __my_class_id = __class_id++;                         \
        static constexpr asciis __my_field_name[] =                             \
            { __xlist__(__xitem_, __VA_ARGS__)  ""/*dummy*/ };                  \
        template<class, class>                                                  \
        friend struct ::mixc::macro_xtypeid::__typeid;                          \
        template<class>                                                         \
        friend struct ::mixc::gc_private_tuple::tuple;                          \
        template<uxx __end>                                                     \
        void operator()(__dph<__end>);                                          \
        template<uxx __foo>                                                     \
        static decltype(nullptr) __field_name(__dph<__foo>);                    \
    public:                                                                     \
    using base_list = __bl<                                                     \
        void/*ignore*/ __xlist__(__xbase_, __VA_ARGS__)                         \
    >;                                                                          \
    using member_list_partial = __mlp<                                          \
        nullptr/*ignore*/ __xlist__(__xmlist_, __VA_ARGS__)                     \
    >;                                                                          \
    using member_list = __dfc<                                                  \
        __xlist__(__xplaced_, __VA_ARGS__)                                      \
    >;

#define $   private: static void __field_name(__dph<(__COUNTER__ - __start) / 4>);  };

#define __get_core__(count,get_modify,name,...)                                 \
private:                                                                        \
    static constexpr auto __field_name(__dph<(count - __start) / 4>) {          \
        return #name;                                                           \
    }                                                                           \
get_modify:                                                                     \
    __VA_ARGS__ name() const {                                                  \
        return the(__dph<(count - __start) / 4>());                             \
    }                                                                           \

#define __get__(get_modify,name,...)                                            \
    __get_core__(__COUNTER__, get_modify, name, __VA_ARGS__)                    \
    final & name(__rr<__VA_ARGS__> * receive){                                  \
        receive[0] = name();                                                    \
        return thex;                                                            \
    }

#define __set__(set_modify,name,...)                                            \
set_modify:                                                                     \
    final & name(__VA_ARGS__ value){                                            \
        the(__dph<(__COUNTER__ - __start) / 4>(), value);                       \
        return thex;                                                            \
    }

#define __getset__(get_modify,set_modify,name,...)                              \
    __get__(get_modify,name,__VA_ARGS__)                                        \
    __set__(set_modify,name,__VA_ARGS__)                                        \
private:

#define xw                                          template<class __type> void operator()(__dph<(__COUNTER__ - __start) / 4>, __type const & value)
#define xr                                          operator()(__dph<(__COUNTER__ - __start) / 4>)

#define xpubget_pubsetx(name,...)                   __getset__(public   , public    , name, __VA_ARGS__) __VA_ARGS__ 
#define xpubget_prosetx(name,...)                   __getset__(public   , protected , name, __VA_ARGS__) __VA_ARGS__ 
#define xpubget_prisetx(name,...)                   __getset__(public   , private   , name, __VA_ARGS__) __VA_ARGS__ 
#define xproget_pubsetx(name,...)                   __getset__(protected, public    , name, __VA_ARGS__) __VA_ARGS__ 
#define xproget_prosetx(name,...)                   __getset__(protected, protected , name, __VA_ARGS__) __VA_ARGS__ 
#define xproget_prisetx(name,...)                   __getset__(protected, private   , name, __VA_ARGS__) __VA_ARGS__ 
#define xpriget_pubsetx(name,...)                   __getset__(private  , public    , name, __VA_ARGS__) __VA_ARGS__ 
#define xpriget_prosetx(name,...)                   __getset__(private  , protected , name, __VA_ARGS__) __VA_ARGS__ 
#define xpriget_prisetx(name,...)                   __getset__(private  , private   , name, __VA_ARGS__) __VA_ARGS__ 

#define xpubget_pubset(name,...)                    xpubget_pubsetx(name, decltype(the_t::p ## name) __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
#define xpubget_proset(name,...)                    xpubget_prosetx(name, decltype(the_t::p ## name) __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
#define xpubget_priset(name,...)                    xpubget_prisetx(name, decltype(the_t::p ## name) __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
#define xproget_pubset(name,...)                    xproget_pubsetx(name, decltype(the_t::p ## name) __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
#define xproget_proset(name,...)                    xproget_prosetx(name, decltype(the_t::p ## name) __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
#define xproget_priset(name,...)                    xproget_prisetx(name, decltype(the_t::p ## name) __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
#define xpriget_pubset(name,...)                    xpriget_pubsetx(name, decltype(the_t::p ## name) __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
#define xpriget_proset(name,...)                    xpriget_prosetx(name, decltype(the_t::p ## name) __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
#define xpriget_priset(name,...)                    xpriget_prisetx(name, decltype(the_t::p ## name) __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }

#define xpubsetx(name,...)                          __set__(public      , name, __VA_ARGS__) private: xlink2(__, ignore__(__COUNTER__)) xlink2(__, ignore__(__COUNTER__)) xw
#define xprosetx(name,...)                          __set__(protected   , name, __VA_ARGS__) private: xlink2(__, ignore__(__COUNTER__)) xlink2(__, ignore__(__COUNTER__)) xw
#define xprisetx(name,...)                          __set__(private     , name, __VA_ARGS__) private: xlink2(__, ignore__(__COUNTER__)) xlink2(__, ignore__(__COUNTER__)) xw

#define xpubset(name,...)                           xpubsetx(name, __VA_ARGS__) { the_t::p ## name = value; }
#define xproset(name,...)                           xprosetx(name, __VA_ARGS__) { the_t::p ## name = value; }
#define xpriset(name,...)                           xprisetx(name, __VA_ARGS__) { the_t::p ## name = value; }

#define xpubgetx(name,...)                          __get__(public      , name, __VA_ARGS__) private: xlink2(__, ignore__(__COUNTER__)) xlink2(__, ignore__(__COUNTER__)) __VA_ARGS__ xr
#define xprogetx(name,...)                          __get__(protected   , name, __VA_ARGS__) private: xlink2(__, ignore__(__COUNTER__)) xlink2(__, ignore__(__COUNTER__)) __VA_ARGS__ xr
#define xprigetx(name,...)                          __get__(private     , name, __VA_ARGS__) private: xlink2(__, ignore__(__COUNTER__)) xlink2(__, ignore__(__COUNTER__)) __VA_ARGS__ xr

#define xpubget(name,...)                           xpubgetx(name, decltype(the_t::p ## name) __VA_ARGS__) { return the_t::p ## name; }
#define xproget(name,...)                           xprogetx(name, decltype(the_t::p ## name) __VA_ARGS__) { return the_t::p ## name; }
#define xpriget(name,...)                           xprigetx(name, decltype(the_t::p ## name) __VA_ARGS__) { return the_t::p ## name; }
#endif
