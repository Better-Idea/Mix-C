#ifndef xpack_memop_cmp
#define xpack_memop_cmp
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_cmp::inc
#include"meta/is_class.hpp"
#include"meta/is_enum.hpp"
#include"meta/is_same.hpp"
#include"meta/is_union.hpp"
#include"meta/has_comparator.hpp"
#include"meta/unsigned_type.hpp"
#include"memop/seqlize.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_cmp::origin{
    template<class a>
    inline ixx cmp(a const & left, a const & right){
        auto l = inc::seqlize(left);
        auto r = inc::seqlize(right);

        for (uxx i = 0; i < l.length(); i++){
            if (ixx s = cmp(l[i], r[i]); s != 0){
                return s;
            }
        }
        return 0;
    }

    template<class a>
    inline ixx cmp_with_operator(a const & left, a const & right){
        if constexpr (inc::is_same<asciis, a> or inc::is_same<words, a>){
            auto * l        = left;
            auto * r        = right;
            using item_t    = inc::unsigned_type<decltype(l[0])>;

            for(; l[0] != '\0'; l++, r++){
                if (l[0] != r[0]){
                    break;
                }
            }
            return ixx(item_t(l[0]) - item_t(r[0]));
        }
        else if constexpr (inc::has_comparator<a>){
            return
                left > right ? 1 : 
                left < right ? -1 : 0;
        }
        else{
            return cmp(left, right);
        }
    }
}

#define __xcmpop__(friend_token,...)                                                            \
friend_token inline bool operator >  (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return ::mixc::memop_cmp::origin::cmp(left, right) > 0;                                     \
}                                                                                               \
friend_token inline bool operator >= (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return ::mixc::memop_cmp::origin::cmp(left, right) >= 0;                                    \
}                                                                                               \
friend_token inline bool operator <  (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return ::mixc::memop_cmp::origin::cmp(left, right) <  0;                                    \
}                                                                                               \
friend_token inline bool operator <= (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return ::mixc::memop_cmp::origin::cmp(left, right) <= 0;                                    \
}                                                                                               \
friend_token inline bool operator == (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return ::mixc::memop_cmp::origin::cmp(left, right) == 0;                                    \
}                                                                                               \
friend_token inline bool operator != (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return ::mixc::memop_cmp::origin::cmp(left, right) != 0;                                    \
}

#define __xcmpopx__(friend_token,func,...)                                                      \
friend_token inline bool operator >  (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return left.func(right) > 0;                                                                \
}                                                                                               \
friend_token inline bool operator >= (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return left.func(right) >= 0;                                                               \
}                                                                                               \
friend_token inline bool operator <  (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return left.func(right) <  0;                                                               \
}                                                                                               \
friend_token inline bool operator <= (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return left.func(right) <= 0;                                                               \
}                                                                                               \
friend_token inline bool operator == (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return left.func(right) == 0;                                                               \
}                                                                                               \
friend_token inline bool operator != (__VA_ARGS__ const & left, __VA_ARGS__ const & right){     \
    return left.func(right) != 0;                                                               \
}
#endif

#define xcmpop(...)                     __xcmpop__(,__VA_ARGS__)
#define xcmpop_friend(...)              __xcmpop__(friend,__VA_ARGS__)
#define xcmpopx(func,...)               __xcmpopx__(,func, __VA_ARGS__)
#define xcmpopx_friend(func,...)        __xcmpopx__(friend, func, __VA_ARGS__)

xexport_space(mixc::memop_cmp::origin)
