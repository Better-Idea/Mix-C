#ifndef xpack_docker_transmitter
#define xpack_docker_transmitter
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_transmitter::inc
#include"dumb/struct_type.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_transmitter{
    template<class type_t>
    xstruct(
        xtmpl(transmitter, type_t),
        xpubb(inc::struct_type<type_t>),
        xprif(phas_hold_value, bool)
    )
        using final_t = the_t;
    public:
        transmitter(): phas_hold_value(false){}
        transmitter(type_t const & value) : 
            inc::struct_type<type_t>(value), 
            phas_hold_value(true){
        }

        auto & operator=(type_t const & value){
            auto & ori      = *(type_t *)this;
            ori             = value;
            phas_hold_value = true;
            return this[0];
        }

        auto & operator=(transmitter<type_t> const & value){
            this[0] = (inc::struct_type<type_t> &) value;
            phas_hold_value = value.phas_hold_value;
            return this[0];
        }

        xpubget(has_hold_value)
    $
}

#endif

xexport(mixc::docker_transmitter::transmitter)