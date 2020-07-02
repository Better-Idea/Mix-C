#ifndef xpack_macro_xdefer
#define xpack_macro_xdefer
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_xdefer
    #include"mixc.hpp"
    #include"macro/xlink.hpp"

    namespace xuser {
        template<class lambda>
        struct defer : lambda {
            xgc_fields(
                xiam(defer<lambda>)
            );
        public:
            defer(lambda const & call) : 
                lambda(call){
            }
            ~defer(){
                lambda::operator()();
            }
        };

        struct sugar{
            template<class lambda>
            auto operator * (lambda const & relase){
                return defer{ relase };
            }
        };
    }

    #define xdefer      auto && xlink2(__release, __COUNTER__) = ::mixc::macro_xdefer::sugar() * [&]()
    #pragma pop_macro("xuser")
#endif
