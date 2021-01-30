#ifndef xpack_meta_has_constructor
#define xpack_meta_has_constructor
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_constructor::inc
#include"define/base_type.hpp"
#include"macro/xnew.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_constructor{
    template<class object_t, class ... args>
    concept check = requires(object_t, args ... list){
        object_t{list...};
    };

    template<class, class> struct meta;

    template<class object, class ret, class ... args> 
    struct meta<object, ret(args...)>{
        enum{ result = check<object, args...> };
    };
}

namespace mixc::meta_has_constructor::origin{
    template<class object_t, class constructor>
    concept has_constructor = meta<object_t, constructor>::result != 0;
}

#endif

xexport_space(mixc::meta_has_constructor::origin)