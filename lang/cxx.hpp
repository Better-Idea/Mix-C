#ifdef xuser
    #ifndef xusing_lang_cxx
        #include"lang/private/cxx.hpp"
    #endif

    namespace xuser::inc{
        template<class item>
        struct cxx : xusing_lang_cxx::cxx<item, cxx<item>>{
            using xusing_lang_cxx::cxx<item, cxx<item>>::cxx;
        };

        using c08 = cxx<char>;
    }
#endif
