#ifndef xpack_interface_can_callback
#define xpack_interface_can_callback
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::interface_can_callback::inc
#include"macro/xinterface.hpp"
#include"macro/xis_nullptr.hpp"
#include"memop/signature.hpp"
#include"meta/is_same.hpp"
#pragma pop_macro("xuser")

namespace mixc::interface_can_callback::origin{
    template<class func_t> struct icallback;

    template<class ret_t, class ... args_t>
    struct icallback<ret_t(args_t...)>{
    private:
        using func_t        = ret_t(*)(voidp, args_t...);
        using the_t         = icallback<ret_t(args_t...)>;
        voidp   this_ptr    = nullptr;
        func_t  this_func   = nullptr;
    public:
        template<class object_t>
        requires(
            inc::signature<ret_t(args_t...)>::has(& object_t::operator()) or
            inc::is_same<ret_t(*)(args_t...), object_t>
        )
        constexpr icallback(object_t const & this_ref){
            struct closure{
                static ret_t shell(object_t * this_ptr, args_t ... list){
                    return (*this_ptr)(list...);
                }
            };
            
            this_ptr    = xref this_ref;
            this_func   = func_t(xref closure::shell);
        }

        constexpr icallback(){}

        ret_t operator()(args_t ... list) const {
            return this_func(this_ptr, list...);
        }

        xis_nullptr(this_func == nullptr)
    };

    template<class functor_t, class signature_t>
    concept can_callback = requires(functor_t const & call){
        icallback<signature_t>(call);
    };
}

#endif

xexport_space(mixc::interface_can_callback::origin)