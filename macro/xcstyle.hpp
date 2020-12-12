#ifndef xpack_macro_xcstyle
#define xpack_macro_xcstyle

namespace mixc::macro_xcstyle{
    template<class> struct meta;

    template<class ret, class lambda, class ... args>
    struct meta<ret(lambda::*)(args...) const>{
        static ret cstyle(args ... list){
            return lambda()(list...);
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
