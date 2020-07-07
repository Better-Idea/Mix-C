#ifndef xpack_macro_private_callable
#define xpack_macro_private_callable
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_private_callable
    #include"macro/xis_nullptr.hpp"
    #include"macro/xstruct.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_private_callable{
        xstruct(
            xname(callable_t),
            xprof(__object, void *),
            xprof(__func_list, void **)
        )
            callable_t(){
                __object    = nullptr;
                __func_list = nullptr;
            }

            xis_nullptr(__object == nullptr);

            void operator=(decltype(nullptr)){
                __object = nullptr;
            }
        $
    }

#endif

namespace xuser::inc{
    using ::mixc::macro_private_callable::callable_t;
}
