#ifndef xpack_macro_xgc
#define xpack_macro_xgc
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xitf
        #include"macro/private/xlist.hpp"
        #include"macro/private/xprefix.hpp"
        #include"meta_seq/tlist.hpp"
        #include"meta_seq/tmarge.hpp"
        #include"dumb/dummy_t.hpp"
        #include"dumb/partial.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xitf{
        template<class a0 = inc::dummy_t, class ... args>
        inline auto expand_member_list(){
            using current = typename a0::member_list;
            if constexpr (sizeof...(args) == 0){
                return current();
            }
            else{
                using next = decltype(expand_member_list<args...>());
                using set = typename tmarge<current, next>::new_list;
                return set();
            }
        }
    }

    #define xgc(name,...)                                                               \
    struct name :                                                                       \
        mixc::dumb_dummy_t::dummy_t                                                     \
        __xlist__(base_,base_,__VA_ARGS__) {                                            \
    private:                                                                            \
        using __self__ = name __xlist__(none_,none_,__VA_ARGS__);                       \
        using __expand_member_list__ =                                                  \
            decltype(                                                                   \
                mixc::macro_xitf::expand_member_list<mixc::dumb_dummy_t::dummy_t        \
                    __xlist__(member_header_,member_,__VA_ARGS__)                       \
                >()                                                                     \
            );                                                                          \
    public:                                                                             \
        using partial = mixc::dumb_partial::partial<__self__>;                          \


    #define xgc_fields(...)                                                             \
        __xlist__(field_,field_,__VA_ARGS__);                                           \
        using member_list = typename mixc::tmarge<                                      \
            __expand_member_list__,                                                     \
            mixc::tlist<                                                                \
                __xlist__(member_list_header_,member_list_,__VA_ARGS__)                 \
            >                                                                           \
        >::new_list

    #define xgc_end()       }

    #define xthe    (*this)
#endif
