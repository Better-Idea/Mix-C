#ifndef xpack_macro_xstruct
#define xpack_macro_xstruct
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_xstruct
    #include"meta_seq/vlist.hpp"
    #include"meta_seq/vmarge.hpp"
    #include"macro/private/xlist.hpp"
    #include"dumb/place_holder.hpp"
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
                    typename self::member_list_inside, 
                    decltype(list::member_list())...
                >(vlist<>())
            );
        };

        struct supported_struct{};

        template<class type>
        struct fake{
            type * item = nullptr;
        };
    }

    #define __xprefix_base_
    #define __xprefix_base_itm__(...)
    #define __xprefix_base_iam__(...)
    #define __xprefix_base_pub__(...)                   public      __VA_ARGS__,
    #define __xprefix_base_pro__(...)                   protected   __VA_ARGS__,
    #define __xprefix_base_pri__(...)                   private     __VA_ARGS__,
    #define __xprefix_base_has__(...)
    
    #define __xprefix_placed_
    #define __xprefix_placed_itm__(...)
    #define __xprefix_placed_iam__(name,...)            name __VA_ARGS__
    #define __xprefix_placed_pub__(...)                 , __VA_ARGS__
    #define __xprefix_placed_pro__(...)                 , __VA_ARGS__
    #define __xprefix_placed_pri__(...)                 , __VA_ARGS__
    #define __xprefix_placed_has__(...)

    #define __xprefix_struct_
    #define __xprefix_struct_itm__(...)
    #define __xprefix_struct_iam__(name,...)            name
    #define __xprefix_struct_pub__(...)
    #define __xprefix_struct_pro__(...)
    #define __xprefix_struct_pri__(...)
    #define __xprefix_struct_has__(...)
    
    #define __xprefix_the_
    #define __xprefix_the_itm__(...)
    #define __xprefix_the_iam__(name,...)               name __VA_ARGS__
    #define __xprefix_the_pub__(...)
    #define __xprefix_the_pro__(...)
    #define __xprefix_the_pri__(...)
    #define __xprefix_the_has__(...)

    #define __xprefix_field_
    #define __xprefix_field_itm__(name,...)             __VA_ARGS__ name;
    #define __xprefix_field_iam__(...)
    #define __xprefix_field_pub__(...)
    #define __xprefix_field_pro__(...)
    #define __xprefix_field_pri__(...)
    #define __xprefix_field_has__(...)

    #define __xprefix_mlist_
    #define __xprefix_mlist_itm__(name,...)             & the_t::name, 
    #define __xprefix_mlist_iam__(...)
    #define __xprefix_mlist_pub__(...)
    #define __xprefix_mlist_pro__(...)
    #define __xprefix_mlist_pri__(...)
    #define __xprefix_mlist_has__(...)                  & ::mixc::macro_xgc::fake<__VA_ARGS__>::item

    #define xiam(...)                                   iam__(__VA_ARGS__)
    #define xpub(...)                                   pub__(__VA_ARGS__)
    #define xpro(...)                                   pro__(__VA_ARGS__)
    #define xpri(...)                                   pri__(__VA_ARGS__)
    #define xitm(...)                                   itm__(__VA_ARGS__)

    #define xstruct(...)                                                            \
    struct __xlist__(struct_, struct_, __VA_ARGS__) :                               \
           __xlist__(base_, base_, __VA_ARGS__)                                     \
           ::mixc::macro_xstruct::supported_struct{                                 \
        using the_t = __xlist__(the_, the_, __VA_ARGS__);                           \
        private:                                                                    \
            __xlist__(field_, field_, __VA_ARGS__)                                  \
            enum{ __start = __COUNTER__ + 1 };                                      \
        public:                                                                     \
        using member_list_inside = ::mixc::meta_seq_vlist::vlist<                   \
            __xlist__(mlist_, mlist_, __VA_ARGS__)                                  \
            & ::mixc::macro_xstruct::fake<void>::item                               \
        >;                                                                          \
        using member_list = typename ::mixc::macro_xstruct::data_field_collector<   \
            __xlist__(placed_, placed_, __VA_ARGS__)                                \
        >::member_list;

    #define xstruct_end(...)        } __VA_ARGS__;

    #define __getset__(get_modify,set_modify,name,...)                              \
    get_modify:                                                                     \
        decltype(the_t::p ## name) __VA_ARGS__ name() const {                       \
            return the(::mixc::dumb_place_holder::                                  \
                place_holder<(__COUNTER__ - __start) / 4>()                         \
            );                                                                      \
        }                                                                           \
        final & name(decltype(the_t::p ## name) * receive){                         \
            receive[0] = name();                                                    \
            return thex;                                                            \
        }                                                                           \
    set_modify:                                                                     \
        final & name(decltype(the_t::p ## name) __VA_ARGS__ value){                 \
            the(::mixc::dumb_place_holder::                                         \
                place_holder<(__COUNTER__ - __start) / 4>(), value                  \
            );                                                                      \
            return thex;                                                            \
        }                                                                           \
    private:

    #define xw                                          template<class __type> void operator()(::mixc::dumb_place_holder::place_holder<(__COUNTER__ - __start) / 4>, __type const & value)
    #define xr                                          operator()(::mixc::dumb_place_holder::place_holder<(__COUNTER__ - __start) / 4>)

    #define xpubget_pubsetx(name,...)                   __getset__(public   , public    , name, __VA_ARGS__) decltype(the_t::p ## name) __VA_ARGS__ 
    #define xpubget_prosetx(name,...)                   __getset__(public   , protected , name, __VA_ARGS__) decltype(the_t::p ## name) __VA_ARGS__ 
    #define xpubget_prisetx(name,...)                   __getset__(public   , private   , name, __VA_ARGS__) decltype(the_t::p ## name) __VA_ARGS__ 
    #define xproget_pubsetx(name,...)                   __getset__(protected, public    , name, __VA_ARGS__) decltype(the_t::p ## name) __VA_ARGS__ 
    #define xproget_prosetx(name,...)                   __getset__(protected, protected , name, __VA_ARGS__) decltype(the_t::p ## name) __VA_ARGS__ 
    #define xproget_prisetx(name,...)                   __getset__(protected, private   , name, __VA_ARGS__) decltype(the_t::p ## name) __VA_ARGS__ 
    #define xpriget_pubsetx(name,...)                   __getset__(private  , public    , name, __VA_ARGS__) decltype(the_t::p ## name) __VA_ARGS__ 
    #define xpriget_prosetx(name,...)                   __getset__(private  , protected , name, __VA_ARGS__) decltype(the_t::p ## name) __VA_ARGS__ 
    #define xpriget_prisetx(name,...)                   __getset__(private  , private   , name, __VA_ARGS__) decltype(the_t::p ## name) __VA_ARGS__ 

    #define xpubget_pubset(name,...)                    xpubget_pubsetx(name, __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
    #define xpubget_proset(name,...)                    xpubget_prosetx(name, __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
    #define xpubget_priset(name,...)                    xpubget_prisetx(name, __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
    #define xproget_pubset(name,...)                    xproget_pubsetx(name, __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
    #define xproget_proset(name,...)                    xproget_prosetx(name, __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
    #define xproget_priset(name,...)                    xproget_prisetx(name, __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
    #define xpriget_pubset(name,...)                    xpriget_pubsetx(name, __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
    #define xpriget_proset(name,...)                    xpriget_prosetx(name, __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
    #define xpriget_priset(name,...)                    xpriget_prisetx(name, __VA_ARGS__)  xr { return the_t::p ## name; } xw { the_t::p ## name = value; }
#endif
