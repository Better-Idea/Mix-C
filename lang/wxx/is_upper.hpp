#ifndef xusing_lang_wxx
    #include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_is_upper
#define xpack_lang_wxx_is_upper
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_wxx")
        #undef  xusing_lang_wxx
        #undef  xuser
        #define xuser mixc::lang_wxx_is_upper
        #include"define/base_type.hpp"
        #include"lang/wxx.hpp"
    #pragma pop_macro("xusing_lang_wxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_wxx_is_upper{
        template<class type>
        struct core : inc::wxx<type>{
            using inc::wxx<type>::wxx;
            using the_t = core<type>;

            auto is_upper() const {
                return 'A' <= the.data and the.data <= 'Z';
            }
        };
    }
#endif

namespace mixc::lang_wxx_is_upper::xuser{
    template<class final, class type>
    struct wxx : xusing_lang_wxx::wxx<final, type> {
        using xusing_lang_wxx::wxx<final, type>::wxx;
        using the_t = core<type>;

        bool is_upper() const {
            return the.is_upper();
        }

        final & is_upper(bool * result) const {
            result[0] = the.is_upper();
            return thex;
        }
    };
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_is_upper::xuser
