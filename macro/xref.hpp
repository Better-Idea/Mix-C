#ifndef xpack_macro_xref
#define xpack_macro_xref
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_xref
        #include"memop/addressof.hpp"
    #pragma pop_macro("xuser")
    #define xref ::mixc::memop_addressof::sugar_addressof() *
#endif
