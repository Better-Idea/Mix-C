#ifndef xpack_memop_signature
#define xpack_memop_signature
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::memop_signature
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::memop_signature{
        template<class ret, class ... args>
        struct signature{
            #define xgen(...)                                                               \
                template<class object>                                                      \
                static auto check(ret (object::* this_call)(args...) __VA_ARGS__){          \
                    union {                                                                 \
                        voidp               result;                                         \
                        decltype(this_call) mem;                                            \
                    };                                                                      \
                    mem = this_call;                                                        \
                    return result;                                                          \
                }
            xgen()
            xgen(const)
            #undef xgen

            #define xgen(...)                                                               \
                template<class object>                                                      \
                static constexpr auto has(ret (object::*)(args...) __VA_ARGS__){            \
                    return true;                                                            \
                }
            xgen()
            xgen(const)
            #undef xgen

            static ret call(voidp self, voidp this_call, args ... list){
                union {
                    voidp  mem;
                    ret (* result)(voidp, args...);
                };
                mem = this_call;
                return result(self, list...);
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::memop_signature::signature;
}
