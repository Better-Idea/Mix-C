#ifndef xpack_draft_binarization
#define xpack_draft_binarization
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::draft_binarization::inc
#include"algo/heap_root.hpp"
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::draft_binarization::origin{
    enum class binarization_mode_t{
        draw_white,
        draw_black,
        draw_both,
    };

    template<class matrix_in_t, class rander_t>
    inline void binarization_core(
        matrix_in_t     const & input, 
        uxx                     x_start,
        uxx                     y_start,
        uxx                     x_end,
        uxx                     y_end,
        binarization_mode_t     mode,
        rander_t        const & randerx
    ){
        constexpr uxx r                 = 3;
        constexpr uxx r_r               = r * r;

        struct pair{
            uxx value;
            uxx x;
            uxx y;
        };

        static_assert(uxx(binarization_mode_t::draw_white) + uxx(binarization_mode_t::draw_black) == 1);
        pair heap[r_r];
        auto main_color                 = binarization_mode_t(uxx(mode) & 1);
        auto neg_color                  = binarization_mode_t(uxx(main_color) ^ 1);
        auto compare = 
            mode == binarization_mode_t::draw_black ? 
            inc::default_compare<pair> : 
            inc::default_compare_neg<pair>;

        for(uxx y = y_start; y < y_end; y += r){
            for(uxx x = x_start; x < x_end; x += r){
                uxx length              = 0;
                uxx sum                 = 0;
                uxx h                   = x_end - x > r ? r : x_end - x;
                uxx w                   = y_end - y > r ? r : y_end - y;

                for(uxx i = 0; i < h; i++){
                    for(uxx j = 0; j < w; j++){
                        auto nx         = x + j;
                        auto ny         = y + i;
                        auto v          = input[ny][nx];
                        auto brightness = uxx(v.red() + v.green() + v.blue());
                        sum            += brightness;

                        // TODO：使用 O(1) 的算法 =========================================
                        // 将像素按亮度排序
                        inc::heap_root::push(
                            heap, 
                            length++, 
                            pair{ .value = brightness, .x = nx, .y = ny }, 
                            compare
                        );
                    }
                }

                // 算出总亮度 brightness，然后除以 (3 * 256) 得出该 w * w 的宫格内可以有多少个白色的像素
                auto temp               = ixx(sum / 3);
                auto count              = ixx(temp / 256) + (temp % 256 > 128);

                if (mode == binarization_mode_t::draw_black){
                    count               = r_r - count;
                }

                // 取出前 count 个最亮/暗的像素
                while(count-- > 0){
                    pair point          = inc::heap_root::pop(heap, length--, compare);
                    randerx(point.x, point.y, main_color);
                }

                // 再取出剩下的点作为相反色
                if (length += 1; 
                    mode == binarization_mode_t::draw_both) while(length-- > 0){
                    pair point          = inc::heap_root::pop(heap, length, compare);
                    randerx(point.x, point.y, neg_color);
                }
            }
        }
    }
}

#endif

xexport_space(mixc::draft_binarization::origin)
