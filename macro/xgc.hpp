#ifndef xpack_macro_xgc
#define xpack_macro_xgc
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xgc
        #include"meta/is_class.hpp"
        #include"meta_seq/vmarge.hpp"
        #include"meta_seq/vappend.hpp"
        #include"meta_seq/vlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xtypeid{
        template<class type> union __typeid;
    }

    namespace mixc::macro_xgc {
        template<class a0, class ... args>
        struct first_t {
            using type = a0;
        };

        template<class type>
        struct fake { type * item; };

        template<template<int> class member_list, int i = 0, class vlist = inc::vlist<>>
        inline auto append() {
            if constexpr (member_list<i>::is_empty) {
                return vlist();
            }
            else {
                using new_list = typename inc::vappend<vlist, member_list<i>::gc_item_t>::new_list;
                return append<member_list, i + 1, new_list>();
            }
        }

        template<class a0, class ... args>
        inline auto marge() {
            if constexpr (inc::is_class<a0>){
                using vlist = decltype(append<a0::template gc_member_t>());

                if constexpr (sizeof...(args) == 0) {
                    return vlist();
                }
                else{
                    using tlist_next = decltype(marge<args...>());
                    return typename inc::vmarge<vlist, tlist_next>::new_list();
                }
            }
            else if constexpr (sizeof...(args) == 0) {
                return inc::vlist<>();
            }
            else{
                return marge<args...>();
            }
        }

        inline u32 class_id = u32(-1) >> 1;
    }

    #define __xgc_item__(name,...)                              \
        __VA_ARGS__ name;                                       \
    public:                                                     \
        template<> struct gc_member_t<__start - __COUNTER__>{   \
            static constexpr auto is_empty    = false;          \
            static constexpr auto gc_name     = # name;         \
            static constexpr auto gc_item_t   =                 \
                & the_t::name;                                  \
        }

    #define __xgc_name_xthe(...)    #__VA_ARGS__

    #define xins(...)                                           \
    public:                                                     \
        template<> struct gc_member_t<__start - __COUNTER__>{   \
            static constexpr auto is_empty    = false;          \
            static constexpr auto gc_item_t   =                 \
                & ::mixc::macro_xgc::fake<__VA_ARGS__>::item;   \
        }

    #define xpub(name,...)  public   : __xgc_item__(name,__VA_ARGS__)
    #define xpro(name,...)  protected: __xgc_item__(name,__VA_ARGS__)
    #define xpri(name,...)  private  : __xgc_item__(name,__VA_ARGS__)

    #define xgc_fields(meta,...)                                \
    public:                                                     \
        using the_t =                                           \
            typename ::mixc::macro_xgc::first_t<meta>::type;    \
        template<int i> struct gc_member_t {                    \
            static constexpr auto is_empty = true;              \
            static constexpr auto gc_name = __xgc_name_ ## meta;\
            static inline auto gc_class_id =                    \
                ::mixc::macro_xgc::class_id++;                  \
        };                                                      \
    private:                                                    \
        enum { __start = __COUNTER__ - 1,  };                   \
        __VA_ARGS__                                             \
    public:                                                     \
        using member_list = decltype(                           \
            ::mixc::macro_xgc::marge<meta>()                    \
        )

    #define xthe(...)    __VA_ARGS__
#endif
