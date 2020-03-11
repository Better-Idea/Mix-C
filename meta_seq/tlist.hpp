#pragma once

namespace mixc {
    template<class ... args>
    struct tlist{
        static constexpr uxx length = sizeof...(args);
    };
}
