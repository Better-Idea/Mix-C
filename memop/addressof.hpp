#ifndef xpack_memop_addressof
#define xpack_memop_addressof
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_addressof
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_addressof{
        template<class a> 
        inline a * addressof(a const & value){
            struct inner{
                inner(a const & value) : value(value){ }
                a const & value;
            } got(value);
            return (*(a **)& got);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::memop_addressof::addressof;
}
