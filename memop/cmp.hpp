#ifndef xpack_memop_cmp
#define xpack_memop_cmp
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_cmp::inc
#include"meta/is_class.hpp"
#include"meta/is_enum.hpp"
#include"meta/is_union.hpp"
#include"memop/seqlize.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_cmp{
    template<class a>
    inline ixx cmp(a const & left, a const & right){
        if constexpr (inc::is_class<a> or inc::is_enum<a> or inc::is_union<a>){
            auto l = inc::seqlize(left);
            auto r = inc::seqlize(right);

            for (uxx i = 0; i < l.length(); i++){
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

#define __xcmpop__(friend_token,...)                                                            \
friend_token inline bool operator >  (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return mixc::memop_cmp::cmp(left, right) > 0;                                               \
}                                                                                               \
friend_token inline bool operator >= (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return mixc::memop_cmp::cmp(left, right) >= 0;                                              \
}                                                                                               \
friend_token inline bool operator <  (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return mixc::memop_cmp::cmp(left, right) <  0;                                              \
}                                                                                               \
friend_token inline bool operator <= (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return mixc::memop_cmp::cmp(left, right) <= 0;                                              \
}                                                                                               \
friend_token inline bool operator == (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return mixc::memop_cmp::cmp(left, right) == 0;                                              \
}                                                                                               \
friend_token inline bool operator != (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return mixc::memop_cmp::cmp(left, right) != 0;                                              \
}
#endif

#define xcmpop(...)             __xcmpop__(,__VA_ARGS__)
#define xcmpop_friend(...)      __xcmpop__(friend,__VA_ARGS__)

xexport(mixc::memop_cmp::cmp)
