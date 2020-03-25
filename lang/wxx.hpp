#ifdef xuser
    #ifndef xusing_lang_wxx
        #include"lang/private/wxx.hpp"
    #endif

    namespace xuser::inc{
        template<class item>
        struct wxx : xusing_lang_wxx::wxx<wxx<item>, item>{
            using xusing_lang_wxx::wxx<wxx<item>, item>::wxx;
        };
    }

    #undef xusing_lang_wxx
#endif
