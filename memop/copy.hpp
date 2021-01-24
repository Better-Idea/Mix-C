#ifndef xpack_memop_copy
#define xpack_memop_copy
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_copy::inc
#include"dumb/mirror.hpp"
#include"memop/addressof.hpp"
#include"memop/zeros.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_copy::origin{
    template<class a, class b = a>
    inline void copy(a * des, b const & src){
        using mp = inc::mirror<a> *;
        enum{
            dis = sizeof(a) - sizeof(b)
        };
        auto m   = *mp(xref src);
        
        // 高位 0 扩展
        if constexpr (dis > 0){
            inc::zeros(u08p(xref m) + sizeof(b), dis);
        }
        *mp(des) = m;
    }

    constexpr bool with_operator = true;
    constexpr bool is_safe       = true;

    template<bool is_safe, bool with_operator, class a, class b>
    inline void copy_core(a & target, b const & source, uxx count) {
        if constexpr (is_safe){
            struct itr{ 
                uxx begin; uxx end; uxx step;
            };

            itr i = uxx(xref target[0]) > uxx(xref source[0]) ? 
                itr{ count - 1, uxx(-1), uxx(-1) } :
                itr{ 0, count, 1 };

            for(; i.begin != i.end; i.begin += i.step){
                if constexpr (with_operator){
                    target[i.begin] = source[i.begin];
                }
                else{
                    copy(xref target[i.begin], source[i.begin]);
                }
            }
        }
        else{
            while(count-- > 0){
                if constexpr (with_operator){
                    target[count] = source[count];
                }
                else{
                    copy(xref target[count], source[count]);
                }
            }
        }
    }

    #define xgen(name,is_safe,with_opr)                                         \
    template<class a, class b = a>                                              \
    inline void name(a & target, b const & source, uxx count){                  \
        copy_core<is_safe, with_opr, a, b>((a &)target, source, count);         \
    }                                                                           \
                                                                                \
    template<class a, class b = a>                                              \
    inline void name(a && target, b const & source, uxx count){                 \
        name((a &)target, source, count);                                       \
    }

    // 无运算符重载的拷贝
    // 适合于无线程竞争的环境
    xgen(copy                     ,     is_safe, not with_operator)

    // 带运算符重载的拷贝(更安全，但可能会引入额外的复杂度)
    xgen(copy_with_operator       ,     is_safe,     with_operator)

    // 不安全版本
    // 无重合检查，适合于两个没有交集数组间的拷贝操作
    xgen(copy_unsafe              , not is_safe, not with_operator)
    xgen(copy_with_operator_unsafe, not is_safe,     with_operator)
    #undef xgen
}

#endif

xexport_space(mixc::memop_copy::origin)