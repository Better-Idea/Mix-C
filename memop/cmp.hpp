#ifndef xpack_memop_cmp
#define xpack_memop_cmp
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_cmp
        #include"define/base_type.hpp"
        #include"meta/is_class.hpp"
        #include"meta/is_enum.hpp"
        #include"meta/is_union.hpp"
        #include"memop/base_seqlize.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_cmp{
        template<class a>
        inline ixx cmp(a const & left, a const & right){
            if constexpr(inc::is_class<a> or inc::is_enum<a> or inc::is_union<a>){
                auto l = inc::base_seqlize(left);
                auto r = inc::base_seqlize(right);
                for (auto i = 0; i < l.length; i++){
                    if (ixx s = cmp(l.ptr[i], r.ptr[i]); s != 0){
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
    using ::mixc::memop_cmp::cmp;
}

