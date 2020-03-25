#ifndef xpack_lang_wxx
#define xpack_lang_wxx
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lang_wxx
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lang_wxx{
        template<class type>
        xgc(core)
            xgc_fields(
                xpro(data, type)
            );
        public:
            core() = default;
            core(core const &) = default;
            core(type value) : data(value){}
        xgc_end();

        template<class final, class type>
        struct wxx : core<type> {
            using core<type>::core;
        };
    }
#endif

#define xusing_lang_wxx     mixc::lang_wxx
