#ifndef xpack_memop_cast
#define xpack_memop_cast
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_cast
        #include"define/base_type.hpp"
        #include"memop/addressof.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_cast{
        template<class target, class source> 
        inline target & cast(source const & value){
            return *(target *)inc::addressof(value);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::memop_cast::cast;
}
