#ifndef xpack_memop_addressof
#define xpack_memop_addressof
#include"macro/xexport.hpp"

namespace mixc::memop_addressof{
    template<class type> 
    inline type * addressof(type const & value){
        struct inner{
            inner(type & value) : value(value){ }
            type & value;
        } got((type &)value);
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

xexport(mixc::memop_addressof::addressof)