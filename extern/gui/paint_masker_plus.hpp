#ifndef xpack_extern_gui_paint_masker_plus
#define xpack_extern_gui_paint_masker_plus
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::extern_gui_paint_masker_plus
    #include"mixc.hpp"
    #include"docker/bit_indicator.hpp"
    #include"extern/gui/private/addressing.hpp"
    #include"extern/gui/private/try_draw_result_t.hpp"
    #include"math/index_system.hpp"
    #include"memop/swap.hpp"

    namespace xuser::origin {
        using inc::try_paint_result_t;

        template<uxx width, uxx height>
        struct paint_masker_plus : inc::addressing<width, height>{
        private:
            using masker = inc::bit_indicator<width * height>;
            masker   idc[2];
            masker * idc_a;
            masker * idc_b;
        public:
            paint_masker_plus() : 
                idc_a(idc), idc_b(idc + 1){
            }

            try_paint_result_t try_paint(uxx x, uxx y){
                auto index = addressing(x, y);

                if (idc_a->get(index) == false){
                    idc_a->set(index);
                    idc_b->reset(index);
                    return try_paint_result_t::can_draw;
                }
                return try_paint_result_t::cannot_draw;
            }

            template<class callback>
            void mask(callback const & clean){
                inc::swap(xref idc_a, xref idc_b);
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
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc {
    using namespace ::mixc::extern_gui_paint_masker_plus::origin;
}
