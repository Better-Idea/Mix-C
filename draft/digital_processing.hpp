#ifndef xpack_draft_digital_processing
#define xpack_draft_digital_processing
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::draft_digital_processing::inc
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::draft_digital_processing{
    template<class item_t>
    void triangular_wave(item_t * buffer, uxx length, uxx scale, uxx asymmetry) {
        // 构建定点数
        auto shift      = (sizeof(uxx) * 8 / 2);
        auto h          = (asymmetry) << shift;
        auto h2         = (h * 2);
        auto accumulate = (0);
        auto cycle      = (length / scale);
        auto step       = (h2 / scale);

        // TODO：不足一个周期的首部

        for(auto i = 0; i < cycle; i++, accumulate -= h2) {
            while(accumulate <= h) {
                buf[0]      = accumulate >> shift;
                buf        += 1;
                accumulate += step;
            }

            // 对称性
            while (accumulate <= h2) {
                buf[0]      = (h2 - accumulate) >> shift;
                buf        += 1;
                accumulate += step;
            }
        }

        // TODO：不足一个周期的尾部
    }
}

#endif
