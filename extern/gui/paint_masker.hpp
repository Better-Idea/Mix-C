#ifndef xpack_extern_gui_paint_masker
#define xpack_extern_gui_paint_masker
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_gui_paint_masker::inc
#include"extern/gui/private/addressing.hpp"
#include"extern/gui/private/try_paint_result_t.hpp"
#include"math/index_system.hpp"
#include"mixc.hpp"
#include"utils/bit_indicator.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_gui_paint_masker::origin {
    using inc::try_paint_result_t;

    template<uxx width, uxx height>
    xstruct(
        xtmpl(paint_masker, width, height),
        xpubb(inc::addressing<width, height>)
    )
    private:
        inc::bit_indicator<width * height> idc;
    public:
        try_paint_result_t try_paint(uxx x, uxx y){
            auto index = the(x, y);

            if (idc.get(index) == false){
                idc.set(index);
                return try_paint_result_t::can_draw;
            }
            return try_paint_result_t::cannot_draw;
        }

        template<class callback>
        void mask(callback const & clean){
            uxx start = 0;
            uxx end   = 0;

            while(end != not_exist){
                if (end = idc.pop_first(); end == start){
                    start += 1;
                }
                else{
                    clean(co{start, end});
                    start  = end + 1;
                }
            }
        }
    $
}

#endif

xexport_space(mixc::extern_gui_paint_masker::origin)
