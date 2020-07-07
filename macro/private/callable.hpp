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
            xiam(callable_t)
        )
        protected:
            void  *   __object;
            void  **  __func_list;
        public:
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
