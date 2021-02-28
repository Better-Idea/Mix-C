#ifndef xpack_memop_addressof
#define xpack_memop_addressof
#include"macro/xexport.hpp"

namespace mixc::memop_addressof{
    template<class type_t> 
    inline type_t * addressof(type_t const & value){
        struct inner{
            inner(type_t & value) : value(value){ }
            type_t & value;
        } got((type_t &)value);
        return (*(type_t **)& got);
    }

    struct sugar{
        template<class type_t>
        auto * operator * (type_t const & value){
            return addressof(value);
        }
    };
}

#define xref ::mixc::memop_addressof::sugar() *
#endif

xexport(mixc::memop_addressof::addressof)