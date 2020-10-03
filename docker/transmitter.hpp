#ifndef xpack_docker_transmitter
#define xpack_docker_transmitter
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_transmitter
#include"define/base_type.hpp"
#include"dumb/struct_type.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_transmitter{
    template<class type>
    xstruct(
        xtmpl(transmitter, type),
        xpubb(inc::struct_type<type>),
        xprif(phas_hold_value, bool)
    )
        using final = the_t;
    public:
        transmitter(): phas_hold_value(false){}
        transmitter(type const & value) : 
            inc::struct_type<type>(value), 
            phas_hold_value(true){
        }

        auto & operator=(type const & value){
            auto & ori      = *(type *)this;
            ori             = value;
            phas_hold_value = true;
            return this[0];
        }

        auto & operator=(transmitter<type> const & value){
            this[0] = (inc::struct_type<type> &) value;
            phas_hold_value = value.phas_hold_value;
            return this[0];
        }

        xpubget(has_hold_value)
    $
}

#endif

namespace xuser::inc{
    using ::mixc::docker_transmitter::transmitter;
}
