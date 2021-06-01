#ifndef xpack_math_index_system
#define xpack_math_index_system
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::math_index_system
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#include"macro/xindex_rollback.hpp"
#include"macro/xinterface.hpp"
#pragma pop_macro("xuser")

namespace mixc::math_index_system::origin{
    xinterface(
        xname(iinterval),
        xfunc(normalize, void(uxx length)),
        xfunc(left,  uxx()),
        xfunc(right, uxx())
    );

    template<class object_t>
    concept can_interval = requires(object_t object){
        iinterval(object);
    };

    // bend close iinterval
    xstruct(
        xname(cc),
        xprof(m_left,    ixx),
        xprof(m_right,   ixx)
    )
        using final_t = cc;

        template<class left_t, class right_t = ixx>
        cc(left_t left, right_t right = right_t(-1)) : 
            m_left(ixx(left)), m_right(ixx(right)){}

        void normalize(uxx length) const {
            xindex_rollback(length, m_left,  +1);
            xindex_rollback(length, m_right, +1);
        }

        xpubget_prosetx(left, uxx)
            xr{ return uxx(m_left); }
            xw{ m_left = ixx(value); }

        xpubget_prosetx(right, uxx)
            xr{ return uxx(m_right); }
            xw{ m_right = ixx(value); }
    $

    // left close right open iinterval
    struct co : cc{
        using cc::cc;

        void normalize(uxx length) const {
            cc::normalize(length);
            m_right -= m_left <= m_right ? 1 : -1;
        }
    };

    // left open right close iinterval
    struct oc : cc{
        using cc::cc;

        void normalize(uxx length) const {
            cc::normalize(length);
            m_left += m_left <= m_right ? 1 : -1;
        }
    };

    // bend open iinterval
    struct oo : cc{
        using cc::cc;

        void normalize(uxx length) const {
            cc::normalize(length);
            auto asc = m_left <= m_right;

            m_left  += asc ? 1 : -1;
            m_right -= asc ? 1 : -1;
        }
    };
}

namespace mixc{
    using namespace mixc::math_index_system::origin;
}
using namespace mixc::math_index_system::origin;

#endif

xexport_space(mixc::math_index_system::origin)