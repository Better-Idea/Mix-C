#ifndef xpack_extern_gui_paint_masker
#define xpack_extern_gui_paint_masker
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::extern_gui_paint_masker
    #include"mixc.hpp"
    #include"docker/bit_indicator.hpp"
    #include"extern/gui/private/addressing.hpp"
    #include"extern/gui/private/try_draw_result_t.hpp"
    #include"math/index_system.hpp"

    namespace xuser::origin {
        using inc::try_paint_result_t;

        template<uxx width, uxx height>
        struct paint_masker : inc::addressing<width, height>{
        private:
            inc::bit_indicator<width * height> idc;
        public:
            try_paint_result_t try_paint(uxx x, uxx y){
                auto index = addressing(x, y);

                if (idc.get(index) == false){
                    idc.set(index);
                    return try_paint_result_t::can_draw;
                }
                return try_paint_result_t::cannot_draw;
            }

            template<class callback>
            void mask(callback const & clean){
                uxx start = 0;
                uxx end   = uxx(-1);
                while(true){
                    do{
                        start = end + 1;
                        end   = idc.pop_first();

                        if (end == not_exist){
                            return;
                        }
                    }while(end == start);
                    
                    clean(co{start, end});
                }
            }
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc {
    using namespace ::mixc::extern_gui_paint_masker::origin;
}
