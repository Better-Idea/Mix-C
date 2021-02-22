#ifndef xpack_dumb_mirror
#define xpack_dumb_mirror
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::dumb_mirror::inc
#include"define/base_type.hpp"
#include"macro/xexport.hpp"
#include"macro/xnew.hpp"
#include"macro/xstruct.hpp"
#include"memop/addressof.hpp"
#pragma pop_macro("xuser")

namespace mixc::dumb_mirror{
    template<class type_t>
    using item_t = u08 [sizeof(type_t)];
}

namespace mixc::dumb_mirror::origin{
    enum class construction_t{
        ignore,
        execute,
    };

    template<class type_t>
    xstruct(
        xtmpl(mirror, type_t),
        xprif(data, item_t<type_t>)
    )
        mirror(): data{0}{}
        mirror(type_t const & value, construction_t mode){
            if (mode == construction_t::ignore){
                this[0] = *(mirror<type_t> *)(xref value);
            }
            else{
                xnew (data) type_t(value);
            }
        }

        void assign(type_t const & value){
            xnew (this) the_t(value, construction_t::ignore);
        }

        void assign_with_operator(type_t const & value){
            operator type_t &() = value;
        }

        type_t * operator->(){
            return (type_t *)data;
        }

        const type_t * operator->() const {
            return (type_t *)data;
        }

        operator type_t & (){
            return *(type_t *)data;
        }

        operator type_t & () const {
            return *(type_t *)data;
        }

        constexpr uxx bytes() const {
            return sizeof(type_t);
        }
    $

    template<>
    xstruct(
        xspec(mirror, void)
    ) $
}

#endif

xexport_space(mixc::dumb_mirror::origin)