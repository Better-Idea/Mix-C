#ifndef xpack_memop_signature
#define xpack_memop_signature
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::memop_signature
#include"configure.hpp"
#include"define/base_type.hpp"
#pragma pop_macro("xuser")

namespace mixc::memop_signature{
    template<class function> struct signature;

    template<class ret, class ... args>
    struct signature<ret(args...)>{
        #define xgen(...)                                                               \
            template<class object>                                                      \
            static auto check(ret (object::* this_call)(args...) __VA_ARGS__){          \
                union {                                                                 \
                    voidp               result;                                         \
                    decltype(this_call) mem;                                            \
                }u;                                                                     \
                u.mem = this_call;                                                      \
                return u.result;                                                        \
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

        static ret call(const void * self, voidp this_call, args ... list){
            typedef struct foo{} * foop;
            union {
                voidp       mem;
                ret (     * gnu_call)(const void *, args...);
                ret (foo::* msvc_call)(args...);
            } u;

            u.mem = this_call;

            // 应对不同平台具有差异的 this_call 
            #if xis_msvc
            return (foop(self)->*u.msvc_call)(list...);
            #else
            return u.gnu_call(self, list...);
            #endif
        }
    };
}

#endif

namespace xuser::inc{
    using ::mixc::memop_signature::signature;
}
