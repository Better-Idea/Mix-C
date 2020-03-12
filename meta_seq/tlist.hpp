#pragma once
#include"define/base_type.hpp"

namespace mixc {
    template<class ... args>
    struct tlist{
        static constexpr uxx length = sizeof...(args);
    };
}
