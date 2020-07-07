/* 注意：
 * 在使用 xpubget_pubsetx 构建属性范式时，请勿使用 __COUNTER__ 
 * 在使用 xstruct 宏时不要出现多余的逗号，像这样
 * xstruct(
 *     xiam(foo),
 *     xitm(bar, type), <- 多余的逗号
 * )
 */

#ifndef xpack_macro_xstruct
#define xpack_macro_xstruct
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_xstruct
    #include"dumb/place_holder.hpp"
    #include"macro/private/xlist.hpp"
    #include"macro/xdummy.hpp"
    #include"meta/remove_ref.hpp"
    #include"meta_seq/tlist.hpp"
    #include"meta_seq/vlist.hpp"
    #include"meta_seq/vmarge.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xstruct{
        using namespace inc;

        template<class self, class ... list>
        struct data_field_collector{
        private:
            template<class a0, class ... args, auto ... result>
            static auto marge(vlist<result...>){
                typename vmarge<vlist<result...>, a0>::new_list items;

                if constexpr (sizeof...(args) == 0){
                    return items;
                }
                else{
                    return marge<args...>(items);
                }
            }
        public:
            using member_list = decltype(marge<
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
        struct fake{
            type * item = nullptr;
        };
    }


    #define __xexpand__
    #define __xexpand_itm__(...)
    #define __xexpand_iam__(...)
    #define __xexpand_imx__(...)
    #define __xexpand_pub__(...)                public      __VA_ARGS__, 
    #define __xexpand_pro__(...)                protected   __VA_ARGS__, 
    #define __xexpand_pri__(...)                private     __VA_ARGS__, 
    #define __xexpand_has__(...)

    #define __xbase__
    #define __xbase_itm__(...)
    #define __xbase_iam__(...)
    #define __xbase_imx__(...)
    #define __xbase_pub__(...)                  , __VA_ARGS__
    #define __xbase_pro__(...)                  , __VA_ARGS__
    #define __xbase_pri__(...)                  , __VA_ARGS__
    #define __xbase_has__(...)
    
    #define __xplaced__
    #define __xplaced_itm__(...)
    #define __xplaced_iam__(name,...)           name __VA_ARGS__
    #define __xplaced_imx__(...)                __VA_ARGS__
    #define __xplaced_pub__(...)                , __VA_ARGS__
    #define __xplaced_pro__(...)                , __VA_ARGS__
    #define __xplaced_pri__(...)                , __VA_ARGS__
    #define __xplaced_has__(...)

    #define __xstruct__
    #define __xstruct_itm__(...)
    #define __xstruct_iam__(name,...)           name
    #define __xstruct_imx__(...)                __VA_ARGS__
    #define __xstruct_pub__(...)
    #define __xstruct_pro__(...)
    #define __xstruct_pri__(...)
    #define __xstruct_has__(...)
    
    #define __xthe__
    #define __xthe_itm__(...)
    #define __xthe_imx__(...)                   __VA_ARGS__
    #define __xthe_iam__(name,...)              name __VA_ARGS__
    #define __xthe_pub__(...)
    #define __xthe_pro__(...)
    #define __xthe_pri__(...)
    #define __xthe_has__(...)

    #define __xfield__
    #define __xfield_itm__(name,...)            __VA_ARGS__ name;
    #define __xfield_imx__(...)
    #define __xfield_iam__(...)
    #define __xfield_pub__(...)
    #define __xfield_pro__(...)
    #define __xfield_pri__(...)
    #define __xfield_has__(...)

    #define __xmlist__
    #define __xmlist_itm__(name,...)            , & the_t::name
    #define __xmlist_imx__(...)
    #define __xmlist_iam__(...)
    #define __xmlist_pub__(...)
    #define __xmlist_pro__(...)
    #define __xmlist_pri__(...)
    #define __xmlist_has__(...)                 , & ::mixc::macro_xstruct::fake<__VA_ARGS__>::item

    #define ximx(...)                           imx__(__VA_ARGS__)
    #define xiam(...)                           iam__(__VA_ARGS__)
    #define xpub(...)                           pub__(__VA_ARGS__)
    #define xpro(...)                           pro__(__VA_ARGS__)
    #define xpri(...)                           pri__(__VA_ARGS__)
    #define xitm(...)                           itm__(__VA_ARGS__)
    #define xhas(...)                           has__(__VA_ARGS__)

    #define xstruct(...)                                                            \
    struct __xlist__(__xstruct_, __VA_ARGS__) :                                     \
           __xlist__(__xexpand_, __VA_ARGS__)                                       \
           ::mixc::dumb_place_holder::place_holder<__COUNTER__> {                   \
        using the_t = __xlist__(__xthe_, __VA_ARGS__);                              \
        private:                                                                    \
            __xlist__(__xfield_, __VA_ARGS__)                                       \
            enum{ __start = __COUNTER__ + 1 };                                      \
        public:                                                                     \
        using base_list = ::mixc::macro_xstruct::base_list<                         \
            void/*ignore*/ __xlist__(__xbase_, __VA_ARGS__)                         \
        >;                                                                          \
        using member_list_partial = mixc::macro_xstruct::member_list_partial<       \
            nullptr/*ignore*/ __xlist__(__xmlist_, __VA_ARGS__)                     \
        >;                                                                          \
        using member_list = typename ::mixc::macro_xstruct::data_field_collector<   \
            __xlist__(__xplaced_, __VA_ARGS__)                                      \
        >::member_list;

    #define $        };

    #define __get__(get_modify,name,...)                                            \
    get_modify:                                                                     \
        __VA_ARGS__ name() const {                                                  \
            return the(::mixc::dumb_place_holder::                                  \
                place_holder<(__COUNTER__ - __start) / 4>()                         \
            );                                                                      \
        }                                                                           \
        final & name(::mixc::meta_remove_ref::remove_ref<__VA_ARGS__> * receive){   \
            receive[0] = name();                                                    \
            return thex;                                                            \
        }                                                                           \

    #define __set__(set_modify,name,...)                                            \
    set_modify:                                                                     \
        final & name(__VA_ARGS__ value){                                            \
            the(::mixc::dumb_place_holder::                                         \
                place_holder<(__COUNTER__ - __start) / 4>(), value                  \
            );                                                                      \
            return thex;                                                            \
        }                                                                           \

    #define __getset__(get_modify,set_modify,name,...)                              \
        __get__(get_modify,name,__VA_ARGS__)                                        \
        __set__(set_modify,name,__VA_ARGS__)                                        \
    private:

    #define xw                                          template<class __type> void operator()(::mixc::dumb_place_holder::place_holder<(__COUNTER__ - __start) / 4>, __type const & value)
    #define xr                                          operator()(::mixc::dumb_place_holder::place_holder<(__COUNTER__ - __start) / 4>)

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

    #define xpubsetx(name,...)                          __set__(public      , name, __VA_ARGS__) private: xw xdummy(__COUNTER__, __COUNTER__)
    #define xprosetx(name,...)                          __set__(protected   , name, __VA_ARGS__) private: xw xdummy(__COUNTER__, __COUNTER__)
    #define xprisetx(name,...)                          __set__(private     , name, __VA_ARGS__) private: xw xdummy(__COUNTER__, __COUNTER__)

    #define xpubset(name,...)                           xpubsetx(name, __VA_ARGS__) { the_t::p ## name = value; }
    #define xproset(name,...)                           xprosetx(name, __VA_ARGS__) { the_t::p ## name = value; }
    #define xpriset(name,...)                           xprisetx(name, __VA_ARGS__) { the_t::p ## name = value; }

    #define xpubgetx(name,...)                          __get__(public      , name, __VA_ARGS__) private: __VA_ARGS__ xr xdummy(__COUNTER__, __COUNTER__)
    #define xprogetx(name,...)                          __get__(protected   , name, __VA_ARGS__) private: __VA_ARGS__ xr xdummy(__COUNTER__, __COUNTER__)
    #define xprigetx(name,...)                          __get__(private     , name, __VA_ARGS__) private: __VA_ARGS__ xr xdummy(__COUNTER__, __COUNTER__)

    #define xpubget(name,...)                           xpubgetx(name,__VA_ARGS__) xr { return the_t::p ## name; }
    #define xproget(name,...)                           xprogetx(name,__VA_ARGS__) xr { return the_t::p ## name; }
    #define xpriget(name,...)                           xprigetx(name,__VA_ARGS__) xr { return the_t::p ## name; }
#endif
