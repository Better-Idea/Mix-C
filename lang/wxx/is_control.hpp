#ifndef xusing_lang_wxx
    #include"lang/private/wxx.hpp"
#endif

#ifndef xpack_lang_wxx_is_control
#define xpack_lang_wxx_is_control
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_wxx")
        #undef  xusing_lang_wxx
        #undef  xuser
        #define xuser mixc::lang_wxx_is_control
        #include"define/base_type.hpp"
        #include"lang/wxx.hpp"
        #include"lang/cxx/index_of_first.hpp"
        #include"lang/cxx.hpp"
    #pragma pop_macro("xusing_lang_wxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_wxx_is_control{
        template<class type>
        struct core : inc::wxx<type>{
            using inc::wxx<type>::wxx;
            using the_t = core<type>;

            auto is_control() const {
                // 9f 1001 1111
                // 7f 0111 1111
                // 1f 0001 1111
                return the.data <= 0x1f or (0x7f <= the.data and the.data <= 0x9f);
            }
        };
    }
#endif

namespace mixc::lang_wxx_is_control::xuser{
    template<class final, class type>
    struct wxx : xusing_lang_wxx::wxx<final, type> {
        using xusing_lang_wxx::wxx<final, type>::wxx;
        using the_t = core<type>;

        auto is_control() const {
            return the.is_control();
        }

        final & is_control(bool * result) const {
            result[0] = the.is_control();
            return thex;
        }
    };
}

#undef  xusing_lang_wxx
#define xusing_lang_wxx ::mixc::lang_wxx_is_control::xuser

