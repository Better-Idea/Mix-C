#ifndef xpack_chrono_private_base_t
#define xpack_chrono_private_base_t
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::chrono_private_base_t
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::chrono_private_base_t{
    template<class final, class value_t = u32>
    xstruct(
        xtmpl(base_t, final, value_t),
        xprof(pvalue, value_t)
    )
        base_t(value_t value = 0) : 
            pvalue(value){}

        operator value_t & (){
            return pvalue;
        }

        operator value_t const & () const {
            return pvalue;
        }

        final & operator = (uxx value){
            pvalue = value;
            return thex;
        }
    $
}

#endif

namespace xuser::inc{
    using ::mixc::chrono_private_base_t::base_t;
}
