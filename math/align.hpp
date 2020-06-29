#ifndef xpack_math_align
#define xpack_math_align
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::math_align
    #include"define/base_type.hpp"
    #include"instruction/index_of_last_set.hpp"

    namespace xuser{
        // 让 size 以 2 的指数次方 向上圆整
        // 例如：
        // align(0x14) = 0x20
        // 注意：
        // align(0) = 0
        // align(v) = 1 if v > 2 exp (sizeof(uxx) * 8 - 1)
        template<class type>
        inline type align(type size){
            return size & (size - 1) ?
                type(1) << (inc::index_of_last_set(size) + 1) :
                size;
        }

        // 让 size 的低 bits 位以 2 的指数次方 向上圆整
        // 例如：
        // align(0x123, 4) = 0x130
        template<class type>
        inline type align(type size, type bits){
            type mask = (type(1) << bits) - 1;
            return (size + mask) & ~mask;
        }
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::math_align::align;
}
