#ifndef xpack_dumb_struct_type
#define xpack_dumb_struct_type
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::dumb_struct_type
#include"define/base_type.hpp"
#include"macro/xstruct.hpp"
#include"meta/is_class.hpp"
#pragma pop_macro("xuser")

namespace mixc::dumb_struct_type{
    template<class type, bool is_class> struct struct_type;
    template<class type>
    xstruct(
        xspec(struct_type, type, true),
        xpubb(type)
    )
        using type::type;

        struct_type(type const & value) : 
            type(value){}
    $

    template<class type>
    xstruct(
        xspec(struct_type, type, false),
        xprif(data, type)
    )
        template<class ... args>
        struct_type(args const & ... list) : 
            data(list...){
        }

        operator type & (){
            return data;
        }

        operator const type & () const {
            return data;
        }
    $

    template<>
    xstruct(
        xspec(struct_type, void, false)
    ) $
}

namespace mixc::dumb_struct_type::origin{
    template<class type>
    using struct_type = mixc::dumb_struct_type::struct_type<type, inc::is_class<type>>;
}

#endif

namespace xuser::inc{
    using ::mixc::dumb_struct_type::origin::struct_type;
}
