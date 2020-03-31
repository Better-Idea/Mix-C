#ifndef xpack_macro_private_callable
#define xpack_macro_private_callable
    namespace mixc::macro_private_callable{
        struct callable_t{
        protected:
            void  *   __object;
            void  **  __func_list;
            using base = callable_t;
        public:
            callable_t(){
                __object    = nullptr;
                __func_list = nullptr;
            }

            bool operator==(decltype(nullptr)) const {
                return __object == nullptr;
            }

            bool operator!=(decltype(nullptr)) const {
                return __object != nullptr;
            }

            friend bool operator==(decltype(nullptr), callable_t const &  value){
                return value == nullptr;
            }

            friend bool operator!=(decltype(nullptr), callable_t const &  value){
                return !(value == nullptr);
            }

            void operator=(decltype(nullptr)){
                __object = nullptr;
            }
        };

        inline const callable_t callable {};
    }

#endif

namespace xuser::inc{
    using ::mixc::macro_private_callable::callable;
    using ::mixc::macro_private_callable::callable_t;
}
