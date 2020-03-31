#pragma once

namespace mixc::macro_xvolatile{
    template<class type>
    inline volatile type & access(type const & value){
        return (volatile type & )value;
    }
}

#define xvolatile(...)  mixc::macro_xvolatile::access(__VA_ARGS__)
