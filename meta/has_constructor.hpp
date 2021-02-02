#ifndef xpack_meta_has_constructor
#define xpack_meta_has_constructor
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_has_constructor::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_has_constructor{
    template<class object_t, class ... args>
    concept meta = requires(object_t, args ... list){
        object_t{list...};
    };

    template<class object, class ... args> 
    inline constexpr bool invoke(object *, void(*)(args...)){
        return meta<object, args...>;
    }
}

namespace mixc::meta_has_constructor::origin{
    template<class object_t, class constructor>
    concept has_constructor = invoke((object_t *)nullptr, (constructor *)nullptr);
}

#endif

xexport_space(mixc::meta_has_constructor::origin)