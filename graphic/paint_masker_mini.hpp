#ifndef xpack_extern_gui_paint_masker_mini
#define xpack_extern_gui_paint_masker_mini
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_gui_paint_masker_mini::inc
#include"define/base_type.hpp"
#include"graphic/private/addressing.hpp"
#include"graphic/private/try_paint_result_t.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"math/index_system.hpp"
#include"utils/bits_indicator.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_gui_paint_masker_mini::origin {
    using inc::try_paint_result_t;

    template<uxx width_v, uxx height_v, uxx rows_v, uxx columns_v>
    requires(
        height_v % rows_v == 0 and width_v % columns_v == 0
    )
    xstruct(
        xtmpl(paint_masker_mini, width_v, height_v, rows_v, columns_v),
        xpubb(inc::addressing<width_v, height_v>)
    )
    private:
        enum{
            pixel_per_row       = height_v / rows_v,
            pixel_per_column    = width_v / columns_v
        };

        using final_t           = the_t;
        using idc_t             = inc::bits_indicator<pixel_per_row * pixel_per_column>;

        idc_t   idc;
        u16     pixel_count[rows_v * columns_v] = {};
        u16     i               = rows_v * columns_v - 1;
    public:
        void analysis(uxx x, uxx y){
            x                  /= pixel_per_column;
            y                  /= pixel_per_row;
            pixel_count[x * columns_v + y]
                               += 1;
        }

        try_paint_result_t try_paint(uxx x, uxx y){
            uxx index;
            uxx r;
            uxx c;
            c                   = x % pixel_per_column;
            x                   = x / pixel_per_column;
            r                   = y % pixel_per_row;
            y                   = y / pixel_per_row;
            index               = x * columns_v + y;

            while (true){
                if (i == not_exist){
                    return try_paint_result_t::cannot_draw;
                }
                if (i -= 1; pixel_count[i] != 0){
                    break;
                }
            }

            if (index != i){
                return try_paint_result_t::cannot_draw;
            }
            if (auto idx = r * pixel_per_column + c; idc.get(idx)){
                return try_paint_result_t::cannot_draw;
            }
            else{
                pixel_count[i] -= 1;
                idc.set(idx);
                return try_paint_result_t::can_draw;
            }
        }

        template<class callback>
        void mask(callback const & clean){
            uxx r;
            uxx c;
            uxx offset;
            uxx start           = 0;
            uxx end             = idc.pop_first();

            for(; end != not_exist; start  += 1, end = idc.pop_first()){
                if (end == start){
                    continue;
                }
                if (end == not_exist){
                    end         = pixel_per_row * pixel_per_column;
                    clean(co{start, end});
                    break;
                }
                else{
                    clean(co{start, end});
                }
            }
        }

        void skip_mask_step(){
            idc.clear();
        }

        xpubgetx(current_block_index, uxx){
            return i;
        }

        xpubgetx(block_begin_x, uxx){
            uxx offset_x        = i % columns_v * pixel_per_column;
            return offset_x;
        }

        xpubgetx(block_begin_y, uxx){
            uxx offset_y        = i / columns_v * pixel_per_row;
            return offset_y;
        }

        xpubgetx(block_width, uxx){
            return pixel_per_column;
        }

        xpubgetx(block_height, uxx){
            return pixel_per_row;
        }

        xpubgetx(need_break, bool){
            return pixel_count[i] == 0;
        }

        xpubgetx(is_finished, bool){
            return i == not_exist;
        }

        xpubgetx(columns, uxx){
            return columns_v;
        }

        xpubgetx(rows, uxx){
            return rows_v;
        }
    $
}

#endif

xexport_space(mixc::extern_gui_paint_masker_mini::origin)
