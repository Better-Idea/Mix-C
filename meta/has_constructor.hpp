#ifndef xpack_meta_has_constructor
#define xpack_meta_has_constructor
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_constructor::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_constructor{
    template<class object_t, class ... args_t>
    concept meta = requires(object_t, args_t ... list){
        object_t{list...};
    };

    template<class object_t, class ... args_t> 
    inline constexpr bool invoke(object_t *, void(*)(args_t...)){
        return meta<object_t, args_t...>;
    }
}

namespace mixc::meta_has_constructor::origin{
    template<class object_t, class constructor_t>
    concept has_constructor = invoke((object_t *)nullptr, (constructor_t *)nullptr);
}

#endif

xexport_space(mixc::meta_has_constructor::origin)