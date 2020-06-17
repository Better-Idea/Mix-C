#ifndef xpack_memop_cmp
#define xpack_memop_cmp
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_cmp
        #include"define/base_type.hpp"
        #include"meta/is_class.hpp"
        #include"meta/is_enum.hpp"
        #include"meta/is_union.hpp"
        #include"memop/seqlize.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_cmp{
        template<class a>
        inline ixx cmp(a const & left, a const & right){
            if constexpr(inc::is_class<a> or inc::is_enum<a> or inc::is_union<a>){
                auto l = inc::seqlize(left);
                auto r = inc::seqlize(right);
                for (auto i = 0; i < l.length(); i++){
                    if (ixx s = cmp(l[i], r[i]); s != 0){
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

    #define xcmpop(type)                                                    \
    inline bool operator >  (type const & left, type const & right){        \
        return mixc::memop_cmp::cmp(left, right) > 0;                       \
    }                                                                       \
    inline bool operator >= (type const & left, type const & right){        \
        return mixc::memop_cmp::cmp(left, right) >= 0;                      \
    }                                                                       \
    inline bool operator <  (type const & left, type const & right){        \
        return mixc::memop_cmp::cmp(left, right) <  0;                      \
    }                                                                       \
    inline bool operator <= (type const & left, type const & right){        \
        return mixc::memop_cmp::cmp(left, right) <= 0;                      \
    }                                                                       \
    inline bool operator == (type const & left, type const & right){        \
        return mixc::memop_cmp::cmp(left, right) == 0;                      \
    }                                                                       \
    inline bool operator != (type const & left, type const & right){        \
        return mixc::memop_cmp::cmp(left, right) != 0;                      \
    }
#endif

namespace xuser::inc{
    using ::mixc::memop_cmp::cmp;
}

