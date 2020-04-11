#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_trim_end
#define xpack_lang_cxx_trim_end
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_trim_end
        #include"define/base_type.hpp"
        #include"lang/cxx/clone.hpp"
        #include"lang/cxx/index_of_last_miss.hpp"
        #include"lang/cxx.hpp"
        #include"lang/private/layout_args.hpp"
        #include"memory/alloc_callback.hpp"
        #include"meta/is_same.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_trim_end{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            template<class ... args>
            auto trim_end(item value, args const & ... list) {
                item           group[sizeof...(args) + 1];
                auto           temp       = the;
                auto           may_alloc  = inc::layout_args(group, value, list...);
                constexpr auto need_alloc = inc::is_same<decltype(may_alloc), inc::alloc_callback<item>>;
                auto           token      = the_t(group, sizeof...(args) + 1 - need_alloc);

                temp.length(
                    temp.index_of_last_miss(
                        token,
                        token.length()
                    )
                );

                if constexpr (not need_alloc){
                    return temp;
                }
                else if (temp.length() == not_exist){
                    return the_t();
                }
                else{
                    return temp.clone(may_alloc);
                }
            }
        };
    }
#endif

namespace mixc::lang_cxx_trim_end::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        template<class ... args>
        final trim_end(item first, args const & ... list) const {
            return the.trim_end(first, list...);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_trim_end::xuser
