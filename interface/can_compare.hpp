#ifndef xpack_interface_can_compare
#define xpack_interface_can_compare
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::interface_can_compare
        #include"define/base_type.hpp"
        #include"interface/can_callback.hpp"
        #include"macro/xitf.hpp"
        #include"macro/xcmp.hpp"
        #include"memop/cmp.hpp"
    #pragma pop_macro("xuser")
    
    namespace mixc::interface_can_compare{
        template<class item>
        using can_compare = inc::can_callback<ixx(item const &, item const &)>;

        template<class type>
        inline auto const & default_compare = 
            [] xcmp(type){
                return inc::cmp<type>(left, right);
            };
    }
#endif

namespace xuser::inc{
    using ::mixc::interface_can_compare::can_compare;
    using ::mixc::interface_can_compare::default_compare;
}
