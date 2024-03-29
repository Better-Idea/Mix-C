#ifndef xpack_extern_gui_paint_masker_plus
#define xpack_extern_gui_paint_masker_plus
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_gui_paint_masker_plus::inc
#include"define/base_type.hpp"
#include"graphic/private/addressing.hpp"
#include"graphic/private/try_paint_result_t.hpp"
#include"macro/xexport.hpp"
#include"macro/xref.hpp"
#include"macro/xstruct.hpp"
#include"math/index_system.hpp"
#include"memop/swap.hpp"
#include"utils/bits_indicator.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_gui_paint_masker_plus::origin {
    using inc::try_paint_result_t;

    template<uxx width, uxx height>
    xstruct(
        xtmpl(paint_masker_plus, width, height),
        xpubb(inc::addressing<width, height>)
    )
    private:
        using masker = inc::bits_indicator<width * height>;
        masker   idc[2];
        masker * idc_a;
        masker * idc_b;
    public:
        paint_masker_plus() : 
            idc_a(idc), idc_b(idc + 1){
        }

        try_paint_result_t try_paint(uxx x, uxx y){
            auto index = the(x, y);

            if (idc_a->get(index) == false){
                idc_a->set(index);
                idc_b->reset(index);
                return try_paint_result_t::can_draw;
            }
            return try_paint_result_t::cannot_draw;
        }

        template<class callback>
        void mask(callback const & clean){
            inc::swap(xref(idc_a), xref(idc_b));
            uxx start = idc_a->pop_first();
            uxx mid   = start;
            uxx end   = 0;

            if (start == not_exist){
                return;
            }

            while(true){
                do{
                    if (end = idc_a->pop_first(); end == not_exist){
                        clean(cc{start, mid});
                        return;
                    }
                    else{
                        mid += 1;
                    }
                }while(end == mid);

                clean(co{start, mid});
                mid = start = end;
            }while(true);
        }
    $
}

#endif

xexport_space(mixc::extern_gui_paint_masker_plus::origin)
