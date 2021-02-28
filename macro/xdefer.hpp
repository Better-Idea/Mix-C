#ifndef xpack_macro_xdefer
#define xpack_macro_xdefer
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_xdefer::inc
#include"mixc.hpp"
#include"macro/xlink.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_xdefer {
    template<class lambda_t>
    struct defer : lambda_t{
        defer(lambda_t const & call) : 
            lambda_t(call){
        }
        ~defer(){
            lambda_t::operator()();
        }
    };

    struct sugar{
        template<class lambda_t>
        auto operator * (lambda_t const & relase){
            return defer{ relase };
        }
    };
}

#define xdefer      [[maybe_unused]] auto && xlink2(__mixc_defer, __LINE__) = ::mixc::macro_xdefer::sugar() * [&]()
#endif
