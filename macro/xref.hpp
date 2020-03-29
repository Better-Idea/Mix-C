#ifndef xpack_macro_xref
#define xpack_macro_xref
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xref
        #include"memop/addressof.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_xref{
        struct sugar{
            template<class type>
            auto * operator * (type const & value){
                return inc::addressof(value);
            }
        };
    }
    #define xref ::mixc::macro_xref::sugar() *
#endif
