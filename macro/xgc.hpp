#ifndef xpack_macro_xgc
#define xpack_macro_xgc
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xgc
        #include"macro/private/xlist.hpp"
        #include"macro/private/xprefix.hpp"
        #include"meta_seq/vlist.hpp"
        #include"meta_seq/vmarge.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xtypeid{
        template<class type> union __typeid;
    }

    namespace mixc::gc_tuple {
        template<class root_t, class member_list> union tuple;
    }

    namespace mixc::macro_xgc{
        template<class a0, class ... args>
        struct first_t {
            using type = a0;
        };

        template<class a0, class ... args>
        inline auto expand_member_list_core(){
            using current = typename a0::member_list;
            if constexpr (sizeof...(args) == 0){
                return current();
            }
            else{
                using next = decltype(expand_member_list_core<args...>());
                using new_list = typename inc::vmarge<current, next>::new_list;
                return new_list();
            }
        }

        template<class dummy, class ... args>
        inline auto expand_member_list() {
            if constexpr (sizeof...(args) == 0) {
                return inc::vlist<>();
            }
            else {
                return expand_member_list_core<args...>();
            }
        }

        template<class type>
        struct fake{
            type item;

            template<class ... args>
            fake(args const & ... list) : item(list...) {}
        };

        inline u32 __class_id = u32(-1) >> 1;
    }
    #define __xgc_name_iam__(...)        #__VA_ARGS__
    #define __xgc_list_iam__(...)        __VA_ARGS__

    #if __clang__ or __GNUC__
        #define __xgc_fields__(meta,...)                                                        \
        __xlist__(field_,field_,__VA_ARGS__)                                                    \
        private:                                                                                \
            using the_t =                                                                       \
                typename ::mixc::macro_xgc::first_t<__xgc_list_ ## meta>::type;                 \
            template<class __type> friend union ::mixc::macro_xtypeid::__typeid;                \
            template<class __root_t, class __member_list> friend union mixc::gc_tuple::tuple;   \
            static constexpr const char * __self_name = __xgc_name_ ## meta;                    \
            static inline auto            __class_id  = ++::mixc::macro_xgc::__class_id;        \
        public:                                                                                 \
            using member_list = typename ::mixc::meta_seq_vmarge::vmarge<                       \
                decltype(::mixc::macro_xgc::expand_member_list<__xgc_list_ ## meta>()),         \
                ::mixc::meta_seq_vlist::vlist<                                                  \
                    __xlist__(first_member_list_,member_list_,__VA_ARGS__)                      \
                >                                                                               \
            >::new_list;
        #define xgc_fields(meta,...)    __xgc_fields__(meta,__VA_ARGS__,)
    #else
        #include"macro/xlink.hpp"
        #define xgc_fields(meta,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32,a33,a34,a35,a36,a37,a38,a39,a40,a41,a42,a43,a44,a45,a46,a47,a48,a49,a50,a51,a52,a53,a54,a55,a56,a57,a58,a59,a60,a61,a62,a63)                                                    \
        __xlist_core__(field_,field_,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32,a33,a34,a35,a36,a37,a38,a39,a40,a41,a42,a43,a44,a45,a46,a47,a48,a49,a50,a51,a52,a53,a54,a55,a56,a57,a58,a59,a60,a61,a62,a63) \
        private:                                                                            \
            using the_t =                                                                   \
                typename ::mixc::macro_xgc::first_t<xlink(__xgc_list_, meta)>::type;        \
            template<class __type> friend union ::mixc::macro_xtypeid::__typeid;            \
            static constexpr const char * __self_name = xlink(__xgc_name_, meta);           \
            static inline auto            __class_id  = ++::mixc::macro_xgc::__class_id;    \
        public:                                                                             \
            using member_list = typename ::mixc::meta_seq_vmarge::vmarge<                   \
                decltype(::mixc::macro_xgc::expand_member_list<xlink(__xgc_list_, meta)>()),\
                ::mixc::meta_seq_vlist::vlist<                                              \
                    __xlist_core__(first_member_list_,member_list_,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15,a16,a17,a18,a19,a20,a21,a22,a23,a24,a25,a26,a27,a28,a29,a30,a31,a32,a33,a34,a35,a36,a37,a38,a39,a40,a41,a42,a43,a44,a45,a46,a47,a48,a49,a50,a51,a52,a53,a54,a55,a56,a57,a58,a59,a60,a61,a62,a63)                  \
                >                                                                           \
            >::new_list;
        #pragma warning(disable:4003)
    #endif

    #define xiam(...)        iam__(__VA_ARGS__)
#endif
