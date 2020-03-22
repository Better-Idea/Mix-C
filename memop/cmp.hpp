#ifndef xpack_memop_cmp
#define xpack_memop_cmp
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_cmp
        #include"define/base_type.hpp"
        #include"memop/base_seqlize.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_cmp{
        template<class a>
        inline ixx cmp(a const & left, a const & right){
            if constexpr(__is_class(a) or __is_union(a) or __is_enum(a)){
                auto l = mixc::base_seqlize(left);
                auto r = mixc::base_seqlize(right);
                for (auto i = 0; i < l.lenght; i++){
                    ixx s = cmp(l.ptr[i], r.ptr[i]);
                    if (s){
                        return s;
                    }
                }
                return 0;
            }
            else{
                return
                    left > right ? 1 : 
                    left < right ? -1 : 0;
            }
        }
    }

#endif

namespace xuser::inc{
    using namespace mixc::memop_cmp;
}

