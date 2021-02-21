#ifndef xpack_extern_gui_private_try_paint_result_t
#define xpack_extern_gui_private_try_paint_result_t
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_gui_private_try_paint_result_t::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_gui_private_try_paint_result_t {
    enum class try_paint_result_t{
        cannot_draw,
        can_draw,
    };
}

#endif

xexport(mixc::extern_gui_private_try_paint_result_t::try_paint_result_t)
