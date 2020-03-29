#ifndef xpack_memop_addressof
#define xpack_memop_addressof
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_addressof
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_addressof{
        template<class type> 
        inline type * addressof(type const & value){
            struct inner{
                inner(type const & value) : value(value){ }
                type const & value;
            } got(value);
            return (*(type **)& got);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::memop_addressof::addressof;
}
