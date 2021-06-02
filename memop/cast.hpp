#ifndef xpack_memop_cast
#define xpack_memop_cast
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_cast::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xref.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_cast{
    template<class target_t, class source_t> 
    inline target_t & cast(source_t const & value){
        return *(target_t *)xref(value);
    }
}

#endif

xexport(mixc::memop_cast::cast)