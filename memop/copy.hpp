#ifndef xpack_memop_copy
#define xpack_memop_copy
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_copy::inc
#include"dumb/mirror.hpp"
#include"macro/xexport.hpp"
#include"macro/xref.hpp"
#include"memop/zeros.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_copy{
    template<class a0_t, class a1_t = a0_t>
    inline void copy(a0_t * des, a1_t const & src){
        using mp = inc::mirror<a0_t> *;
        enum : ixx{
            dis = ixx(sizeof(a0_t) - sizeof(a1_t))
        };

        auto m   = *mp(xref(src));
        
        // 高位 0 扩展
        if constexpr (dis > 0){
            inc::zeros(u08p(xref(m)) + sizeof(a1_t), dis);
        }
        *mp(des) = m;
    }

    constexpr bool with_operator = true;
    constexpr bool is_safe       = true;

    template<bool is_safe_v, bool with_operator_v, class a0_t, class a1_t>
    inline void copy_core(a0_t & target, a1_t const & source, uxx count) {
        if constexpr (is_safe_v){
            struct itr{ 
                uxx begin; uxx end; uxx step;
            };

            itr i = uxx(xref(target[0])) > uxx(xref(source[0])) ? 
                itr{ count - 1, uxx(-1), uxx(-1) } :
                itr{ 0, count, 1 };

            for(; i.begin != i.end; i.begin += i.step){
                if constexpr (with_operator_v){
                    target[i.begin] = source[i.begin];
                }
                else{
                    copy(xref(target[i.begin]), source[i.begin]);
                }
            }
        }
        else{
            while(count-- > 0){
                if constexpr (with_operator_v){
                    target[count] = source[count];
                }
                else{
                    copy(xref(target[count]), source[count]);
                }
            }
        }
    }
}

namespace mixc::memop_copy::origin{
    using mixc::memop_copy::copy;

    #define xgen(name,is_safe,with_opr)                                                 \
    template<class a0_t, class a1_t = a0_t>                                             \
    inline void name(a0_t & target, a1_t const & source, uxx count){                    \
        copy_core<is_safe, with_opr, a0_t, a1_t>((a0_t &)target, source, count);        \
    }                                                                                   \
                                                                                        \
    template<class a0_t, class a1_t = a0_t>                                             \
    inline void name(a0_t && target, a1_t const & source, uxx count){                   \
        name((a0_t &)target, source, count);                                            \
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