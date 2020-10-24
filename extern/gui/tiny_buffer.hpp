#ifndef xpack_extern_gui_tiny_buffer
#define xpack_extern_gui_tiny_buffer
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_gui_tiny_buffer
#include"docker/bit_indicator.hpp"
#include"extern/gui/private/try_draw_result_t.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_gui_tiny_buffer::origin {
    using inc::try_draw_result_t;

    template<uxx width, uxx height, uxx rows, uxx columns>
    xstruct(
        xtmpl(tiny_buffer, width, height, rows, columns)
    )
    private:
        enum{
            pixel_per_rows      = height / rows,
            pixel_per_columns   = width / columns
        };

        using idc_t                         = inc::bit_indicator<pixel_per_rows * pixel_per_columns>;
        idc_t   idc;
        uxx     pixel_count[rows * columns] = {};
        uxx     i                           = rows * columns - 1;

    public:
        void analysis(uxx x, uxx y){
            x                              /= pixel_per_columns;
            y                              /= pixel_per_row;
            pixel_count[x * columns + y]   += 1;
        }

        try_draw_result_t try_paint(uxx x, uxx y){
            uxx index;
            uxx r;
            uxx c;
            x                              /= pixel_per_columns;
            c                              %= pixel_per_columns;
            y                              /= pixel_per_row;
            r                              %= pixel_per_row;
            index                           = x * columns + y;

            while (true){
                if (i == not_exist){
                    return try_draw_result_t::cannot_draw;
                }
                if (i -= 1; pixel_count[i] != 0){
                    break;
                }
            }

            if (index != i){
                return try_draw_result_t::cannot_draw;
            }
            if (auto idx = r * pixel_per_columns + c; idc.get(idx)){
                return try_draw_result_t::cannot_draw;
            }
            else{
                pixel_count[i]             -= 1;
                idc.set(idx);
                return try_draw_result_t::can_draw;
            }
        }

        xpubgetx(need_break, bool){
            return pixel_count[i] == 0;
        }

        xpubgetx(is_finished, bool){
            return i == not_exist;
        }
    $
}

#endif
