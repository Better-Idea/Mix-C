#ifndef xpack_extern_gui_private_try_paint_result_t
#define xpack_extern_gui_private_try_paint_result_t
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::extern_gui_private_try_paint_result_t
    #include"mixc.hpp"

    namespace xuser::origin {
        enum class try_paint_result_t{
            cannot_draw,
            can_draw,
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc {
    using ::mixc::extern_gui_private_try_paint_result_t::origin::try_paint_result_t;
}
