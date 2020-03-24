#ifndef xpack_macro_xgc
#define xpack_macro_xgc
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xgc
        #include"dumb/place_holder.hpp"
        #include"macro/private/xlist.hpp"
        #include"macro/private/xprefix.hpp"
        #include"meta_seq/vlist.hpp"
        #include"meta_seq/vmarge.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xtypeid{
        template<class type> union __typeid__;
    }

    namespace mixc::macro_xgc{
        struct none{
            using member_list = inc::vlist<>;
        };

        template<class a0 = none, class ... args>
        inline auto expand_member_list(){
            using current = typename a0::member_list;
            if constexpr (sizeof...(args) == 0){
                return current();
            }
            else{
                using next = decltype(expand_member_list<args...>());
                using set = typename inc::vmarge<current, next>::new_list;
                return set();
            }
        }
    }

    #define xgc(name,...)                                                               \
    struct name :                                                                       \
        private mixc::dumb_place_holder::place_holder<__COUNTER__, true>                \
        __xlist__(base_,base_,__VA_ARGS__) {                                            \
    private:                                                                            \
        using __self__ = name __xlist__(keep_tmpl_,keep_tmpl_,__VA_ARGS__);             \
        using __expand_member_list__ =                                                  \
            decltype(                                                                   \
                mixc::macro_xgc::expand_member_list<mixc::macro_xgc::none               \
                    __xlist__(member_header_,member_,__VA_ARGS__)                       \
                >()                                                                     \
            );                                                                          \
        template<class __type__> friend union mixc::macro_xtypeid::__typeid__;          \
        static constexpr const char * __self_name__ = # name;                           \
    public:

    #define xgc_fields(...)                                                             \
        __xlist__(field_,field_,__VA_ARGS__);                                           \
        using member_list = typename mixc::meta_seq_vmarge::vmarge<                     \
            __expand_member_list__,                                                     \
            mixc::meta_seq_vlist::vlist<                                                \
                __xlist__(member_list_header_,member_list_,__VA_ARGS__)                 \
            >                                                                           \
        >::new_list

    #define xgc_end()       }
#endif
