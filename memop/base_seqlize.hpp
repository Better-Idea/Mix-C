#ifndef xpack_memop_base_seqlize
#define xpack_memop_base_seqlize
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_base_seqlize
        #include"define/base_type.hpp"
        #include"memop/addressof.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_base_seqlize{
        // 把type转换成指定长度的无符号整数指针
        template<class type>
        inline auto base_seqlize(type const & value){
            #define xgen(type,scale,...)                                              \
                if constexpr (sizeof(value) % scale == 0 __VA_ARGS__){                \
                    struct seg{                                                       \
                        type * ptr;                                                   \
                        uxx    length;                                                \
                    } r { (type *) inc::addressof(value), sizeof(value) / scale };    \
                    return r;                                                         \
                } else
            
            xgen(u64, 8)
            xgen(u32, 4)
            xgen(u16, 2)
            xgen(u08, 1, || true);
            #undef xgen
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::memop_base_seqlize::base_seqlize;
}
