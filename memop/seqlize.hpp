#ifndef xpack_memop_seqlize
#define xpack_memop_seqlize
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_seqlize
        #include"define/base_type.hpp"
        #include"memop/addressof.hpp"
        #include"interface/seqptr.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_seqlize{
        // 把type转换成指定长度的无符号整数指针
        template<class type>
        inline auto seqlize(type const & value){
            #define xgen(size)                                              \
            if constexpr (sizeof(type) % size == 0){                        \
                return inc::seqptr<type>(xref value, sizeof(type) / size);  \
            } else

            xgen(8)
            xgen(4)
            xgen(2)
            xgen(1);
            #undef xgen
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::memop_seqlize::seqlize;
}
