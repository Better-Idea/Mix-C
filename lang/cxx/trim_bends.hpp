#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_trim_bends
#define xpack_lang_cxx_trim_bends
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_trim_bends
        #include"define/base_type.hpp"
        #include"lang/cxx/clone.hpp"
        #include"lang/cxx/index_of_first_miss.hpp"
        #include"lang/cxx/index_of_last_miss.hpp"
        #include"lang/cxx.hpp"
        #include"lang/private/layout_args.hpp"
        #include"memory/alloc_callback.hpp"
        #include"meta/is_same.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_trim_bends{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            template<class ... args>
            auto trim_bends(item first, args const & ... list) const {
                item group[sizeof...(args) + 1]; // 包含'\0'

                auto            may_alloc   = inc::layout_args(group, first, list...);
                constexpr auto  need_alloc  = inc::is_same<decltype(may_alloc), inc::alloc_callback<item>>;
                auto            token       = the_t(group, sizeof...(args) + 1 - need_alloc);
                auto            offset      = the.index_of_first_miss(token, token.length());

                if constexpr (need_alloc){
                    if (offset == not_exist){
                        return inc::cxx<item>();
                    }
                }

                auto temp   = the.backward(offset);
                temp.length() = temp.index_of_last_miss(
                    token,
                    token.length()
                );

                if constexpr (need_alloc){
                    temp = temp.clone(may_alloc);
                }
                return temp;
            }
        };
    }
#endif

namespace xuser::com::lang_cxx_trim_bends{
    namespace cur{ using namespace mixc::lang_cxx_trim_bends; }
    namespace inc{ using namespace cur::inc; }

    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = cur::core<item>;

        template<class ... args>
        final trim_bends(item first, args const & ... list) const {
            return the.trim_bends(first, list...);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::com::lang_cxx_trim_bends
