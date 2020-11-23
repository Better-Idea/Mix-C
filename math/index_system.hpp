#ifndef xpack_math_index_system
#define xpack_math_index_system
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_index_system
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xindex_rollback.hpp"
#include"macro/xinterface.hpp"
#include"macro/xnew.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_index_system::origin{
    xinterface(
        xname(interval),
        xfunc(normalize, void(uxx length)),
        xfunc(left,  uxx()),
        xfunc(right, uxx())
    );

    template<class object_t>
    concept can_interval = requires(object_t object, interval * ptr){
        xnew(ptr) interval(object);
    };

    // bend close interval
    xstruct(
        xname(cc),
        xprof(pleft,   mutable ixx),
        xprof(pright,  mutable ixx)
    )
        using final = cc;

        template<class left_t, class right_t = ixx>
        cc(left_t left, right_t right = right_t(-1)) : 
            pleft(ixx(left)), pright(ixx(right)){}

        void normalize(uxx length) const {
            xindex_rollback(length, pleft,  +1);
            xindex_rollback(length, pright, +1);
        }

        xpubget_prosetx(left, uxx)
            xr{ return uxx(pleft); }
            xw{ pleft = ixx(value); }

        xpubget_prosetx(right, uxx)
            xr{ return uxx(pright); }
            xw{ pright = ixx(value); }
    $

    // left close right open interval
    struct co : cc{
        using cc::cc;

        void normalize(uxx length) const {
            cc::normalize(length);
            pright -= pleft <= pright ? 1 : -1;
        }
    };

    // left open right close interval
    struct oc : cc{
        using cc::cc;

        void normalize(uxx length) const {
            cc::normalize(length);
            pleft += pleft <= pright ? 1 : -1;
        }
    };

    // bend open interval
    struct oo : cc{
        using cc::cc;

        void normalize(uxx length) const {
            cc::normalize(length);
            auto asc = pleft <= pright;

            pleft  += asc ? 1 : -1;
            pright -= asc ? 1 : -1;
        }
    };
}

namespace mixc{
    using namespace mixc::math_index_system::origin;
}
using namespace mixc::math_index_system::origin;

#endif

xexport_space(mixc::math_index_system::origin)