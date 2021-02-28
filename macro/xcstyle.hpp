#ifndef xpack_macro_xcstyle
#define xpack_macro_xcstyle

namespace mixc::macro_xcstyle{
    template<class> struct meta;

    template<class ret_t, class lambda_t, class ... args_t>
    struct meta<ret_t(lambda_t::*)(args_t...) const>{
        static ret_t cstyle(args_t ... list){
            return lambda_t()(list...);
        }
    };

    struct sugar{
        template<class lambda_t>
        auto operator * (lambda_t const &){
            auto func = & lambda_t::operator();
            return meta<decltype(func)>::cstyle;
        }
    };
}

#define xcstyle     ::mixc::macro_xcstyle::sugar() * []
#endif
