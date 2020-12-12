#ifndef xpack_macro_xcall
#define xpack_macro_xcall

namespace mixc::macro_xcall{
    template<class> struct meta;

    template<class ret, class lambda, class ... args>
    struct meta<ret(lambda::*)(args...) const>{
        static ret call(args ... list){
            return lambda()(list...);
        }
    };

    template<class ret, class lambda, class ... args>
    struct meta<ret(lambda::*)(args...)>{
        static ret call(args ... list){
            return lambda()(list...);
        }
    };

    struct sugar{
        template<class lambda_t>
        auto operator * (lambda_t const &){
            auto func = & lambda_t::operator();
            return meta<decltype(func)>::call;
        }
    };
}

#define xcall      ::mixc::macro_xcall::sugar() * []
#endif
