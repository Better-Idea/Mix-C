#ifndef xpack_extern_gui_paint_masker_mini
#define xpack_extern_gui_paint_masker_mini
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_gui_paint_masker_mini::inc
#include"extern/gui/private/addressing.hpp"
#include"extern/gui/private/try_paint_result_t.hpp"
#include"math/index_system.hpp"
#include"mixc.hpp"
#include"utils/bits_indicator.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_gui_paint_masker_mini::origin {
    using inc::try_paint_result_t;

    template<uxx width, uxx height, uxx rows, uxx columns>
    requires(
        height % rows == 0 and width % columns == 0
    )
    xstruct(
        xtmpl(paint_masker_mini, width, height, rows, columns),
        xpubb(inc::addressing<width, height>)
    )
    private:
        enum{
            pixel_per_row                   = height / rows,
            pixel_per_column                = width / columns
        };

        using final_t                         = the_t;
        using idc_t                         = inc::bits_indicator<pixel_per_row * pixel_per_column>;
        idc_t   idc;
        u16     pixel_count[rows * columns] = {};
        u16     i                           = rows * columns - 1;

    public:
        void analysis(uxx x, uxx y){
            x                              /= pixel_per_column;
            y                              /= pixel_per_row;
            pixel_count[x * columns + y]   += 1;
        }

        try_paint_result_t try_paint(uxx x, uxx y){
            uxx index;
            uxx r;
            uxx c;
            x                              /= pixel_per_column;
            c                              %= pixel_per_column;
            y                              /= pixel_per_row;
            r                              %= pixel_per_row;
            index                           = x * columns + y;

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
                pixel_count[i]             -= 1;
                idc.set(idx);
                return try_paint_result_t::can_draw;
            }
        }

        template<class callback>
        void mask(callback const & clean){
            uxx r;
            uxx c;
            uxx offset;
            uxx start       = 0;
            uxx end         = idc.pop_first();

            for(; end != not_exist; start  += 1, end = idc.pop_first()){
                if (end == start){
                    continue;
                }
                if (end == not_exist){
                    end     = pixel_per_row * pixel_per_column;
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
            uxx offset_x    = i % columns * pixel_per_column;
            return offset_x;
        }

        xpubgetx(block_begin_y, uxx){
            uxx offset_y    = i / columns * pixel_per_row;
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
    $
}

#endif

xexport_space(mixc::extern_gui_paint_masker_mini::origin)
