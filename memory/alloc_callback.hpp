#ifndef xpack_memory_alloc_callback
#define xpack_memory_alloc_callback
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memory_alloc_callback
        #include"define/base_type.hpp"
        #include"macro/xitf.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memory_alloc_callback{
        template<class item>
        xitf(alloc_callback, xtmpl(item))
            xitf_item(operator(), item *,
                xnt(count, uxx)
            );
        xitf_end();
    }
#endif

namespace xuser::inc{
    using namespace mixc::memory_alloc_callback;
}
