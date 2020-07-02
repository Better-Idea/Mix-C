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
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc {
    using namespace ::mixc::extern_gui_paint_masker::origin;
}
