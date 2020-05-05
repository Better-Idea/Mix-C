#ifndef xpack_interface_can_seqlize
#define xpack_interface_can_seqlize
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_can_seqlize
        #include"define/base_type.hpp"
        #include"macro/xitf.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::interface_can_seqlize{
        xitf(can_seqlize,
            xtmpl(class item_t),
            xcast(item_t *),
            xitem(length, uxx),
        );
    }

#endif

namespace xuser::inc{
    using ::mixc::interface_can_seqlize::can_seqlize;
}
