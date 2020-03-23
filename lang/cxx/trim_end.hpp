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
        #include"memop/cast.hpp"
        #include"memory/alloc_callback.hpp"
        #include"meta/is_same.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_trim_end{
        template<class item>
        struct cxx : inc::cxx<item>::partial {
            template<class ... args>
            auto trim_end(item value, args const & ... list) {
                item             group[sizeof...(args) + 1];
                inc::cxx<item> & self       = xthe;
                auto             may_alloc  = inc::layout_args(group, value, list...);
                constexpr auto   need_alloc = inc::is_same<decltype(may_alloc), inc::alloc_callback<item>>;
                auto          && token      = inc::cxx<item> { group, sizeof...(args) + 1 - need_alloc };

                self.length = self.index_of_last_miss(
                    token,
                    token.length
                );

                if constexpr (not need_alloc){
                    return self;
                }
                else if (self.length == not_exist){
                    return inc::cxx<item>();
                }
                else{
                    return self.clone(may_alloc);
                }
            }
        };
    }
#endif

namespace xuser::lang_cxx_trim_end{
    namespace cur{
        using namespace mixc::lang_cxx_trim_end;
    }
    namespace inc{
        using namespace cur::inc;
    }

    template<class item, class final>
    struct cxx : xusing_lang_cxx::cxx<item, final> {
        using xusing_lang_cxx::cxx<item, final>::cxx;
        using fun = cur::cxx<item>;

        template<class ... args>
        final trim_end(item first, args const & ... list) const {
            return inc::cast<fun>(xthe).trim_end(first, list...);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx xuser::lang_cxx_trim_end
