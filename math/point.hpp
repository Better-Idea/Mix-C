#ifdef xuser
#ifndef xusing_math_point
#include"math/private/point.hpp"
#endif
#include"macro/xfinal.hpp"

namespace xuser::inc{
    template<class posx_t, class posy_t = posx_t, class posz_t = void>
    xfinal_tmpl(xusing_math_point, point, posx_t, posx_t, posz_t);

    //                  x    y
    using pt2b  = point<u08, u08>;
    using pt2w  = point<u16, u16>;
    using pt2d  = point<u32, u32>;
    using pt2q  = point<u64, u64>;
    using pt2bx = point<i08, i08>;
    using pt2wx = point<i16, i16>;
    using pt2dx = point<i32, i32>;
    using pt2qx = point<i64, i64>;
    using pt2s  = point<f32, f32>;
    using pt2f  = point<f64, f64>;

    //                  x    y    z
    using pt3b  = point<u08, u08, u08>;
    using pt3w  = point<u16, u16, u16>;
    using pt3d  = point<u32, u32, u32>;
    using pt3q  = point<u64, u64, u64>;
    using pt3bx = point<i08, i08, i08>;
    using pt3wx = point<i16, i16, i16>;
    using pt3dx = point<i32, i32, i32>;
    using pt3qx = point<i64, i64, i64>;
    using pt3s  = point<f32, f32, f32>;
    using pt3f  = point<f64, f64, f64>;
}

#undef  xusing_math_point
#endif
