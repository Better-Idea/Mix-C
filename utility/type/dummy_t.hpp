#pragma once
#include"define/base_type.hpp"

namespace mixc{
    // 可用作继承，函数参数占位符等场景的哑元
    template<ixx i = -1>
    struct dummy_t{
        template<class ... args>
        dummy_t(args const & ...){}
    };
}
