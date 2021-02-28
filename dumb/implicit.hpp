/*
用作将指定类型b隐式转换成类型a
场景：
template<class ... args_t>
auto function(args_t const & ... list){
    implicit<int &> seq[] = { list... };
    ...
}
*/
#ifndef xpack_dumb_implicit
#define xpack_dumb_implicit
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::dumb_implicit::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::dumb_implicit{
    template<class a>
    xstruct(
        xtmpl(implicit, a),
        xprif(value, a)
    )
        template<class b>
        implicit(b const & value) : 
            value((a)(b &)value) {}

        operator a () const {
            return value;
        }

        auto operator -> () const {
            return & value;
        }
    $
}

#endif

xexport(mixc::dumb_implicit::implicit)