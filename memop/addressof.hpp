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

        struct sugar{
            template<class type>
            auto * operator * (type const & value){
                return addressof(value);
            }
        };
    }

    #define xref ::mixc::memop_addressof::sugar() *
#endif

namespace xuser::inc{
    using ::mixc::memop_addressof::addressof;
}
