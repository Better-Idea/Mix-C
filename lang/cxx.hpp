#ifdef xuser
    #ifndef xusing_lang_cxx
        #include"lang/private/cxx.hpp"
    #endif

    namespace xuser::inc{
        template<class item>
        struct cxx : xusing_lang_cxx::cxx<cxx<item>, item>{
            using xusing_lang_cxx::cxx<cxx<item>, item>::cxx;
        };

        using c08 = cxx<char>;
    }
    #undef xusing_lang_cxx
#endif
