#ifdef xuser
    #ifndef xusing_lang_wxx
        #include"lang/private/wxx.hpp"
    #endif

    namespace xuser::inc{
        template<class item>
        struct wxx : xusing_lang_wxx::wxx<wxx<item>, item>{
            using xusing_lang_wxx::wxx<wxx<item>, item>::wxx;
        };

        using w08 = wxx<char>;
        using w16 = wxx<char16_t>;
    }

    #undef xusing_lang_wxx
#endif
