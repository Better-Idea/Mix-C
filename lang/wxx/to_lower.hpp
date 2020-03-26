#ifndef xusing_lang_wxx
    #include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_to_lower
#define xpack_lang_wxx_to_lower
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_wxx")
        #undef  xusing_lang_wxx
        #undef  xuser
        #define xuser mixc::lang_wxx_to_lower
        #include"define/base_type.hpp"
        #include"lang/wxx/is_upper.hpp"
        #include"lang/wxx.hpp"
    #pragma pop_macro("xusing_lang_wxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_wxx_to_lower{
        template<class type>
        struct core : inc::wxx<type>{
            using inc::wxx<type>::wxx;
            using the_t = core<type>;

            auto to_lower() const {
                auto dvalue = 'A' - 'a';
                return the.is_upper() ? the.data - dvalue : the.data;
            }
        };
    }
#endif

namespace mixc::lang_wxx_to_lower::xuser{
    template<class final, class type>
    struct wxx : xusing_lang_wxx::wxx<final, type> {
        using xusing_lang_wxx::wxx<final, type>::wxx;
        using the_t = core<type>;

        final to_lower() const {
            return the.to_lower();
        }

        final & to_lower(final * result) const {
            result[0] = the.to_lower();
            return thex;
        }
    };
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_to_lower::xuser
