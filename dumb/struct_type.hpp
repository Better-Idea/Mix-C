#ifndef xpack_dumb_struct_type
#define xpack_dumb_struct_type
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::dumb_struct_type::inc
#include"meta/is_class.hpp"
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xstruct.hpp"
#pragma pop_macro("xuser")

namespace mixc::dumb_struct_type{
    template<class type_t, bool is_class_v> struct struct_type;
    template<class type_t>
    xstruct(
        xspec(struct_type, type_t, true),
        xpubb(type_t)
    )
        using type_t::type_t;

        struct_type(type_t const & value) : 
            type_t(value){}
    $

    template<class type_t>
    xstruct(
        xspec(struct_type, type_t, false),
        xprif(m_value, type_t)
    )
        template<class ... args_t>
        struct_type(args_t const & ... list) : 
            m_value(list...){
        }

        operator type_t & (){
            return m_value;
        }

        operator const type_t & () const {
            return m_value;
        }
    $

    template<>
    xstruct(
        xspec(struct_type, void, false)
    ) $
}

namespace mixc::dumb_struct_type::origin{
    template<class type_t>
    using struct_type = mixc::dumb_struct_type::struct_type<type_t, inc::is_class<type_t>>;
}

#endif

xexport(mixc::dumb_struct_type::origin::struct_type)