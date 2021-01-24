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
    template<class type>
    using item_t = u08 [sizeof(type)];
}

namespace mixc::dumb_mirror::origin{
    enum class construction_t{
        ignore,
        execute,
    };

    template<class type>
    xstruct(
        xtmpl(mirror, type),
        xprif(data, item_t<type>)
    )
        mirror(): data{0}{}
        mirror(type const & value, construction_t mode){
            if (mode == construction_t::ignore){
                this[0] = *(mirror<type> *)(xref value);
            }
            else{
                xnew (data) type(value);
            }
        }

        void assign(type const & value){
            xnew (this) the_t(value, construction_t::ignore);
        }

        void assign_with_operator(type const & value){
            operator type &() = value;
        }

        type * operator->(){
            return (type *)data;
        }

        const type * operator->() const {
            return (type *)data;
        }

        operator type & (){
            return *(type *)data;
        }

        operator type & () const {
            return *(type *)data;
        }

        constexpr uxx bytes() const {
            return sizeof(type);
        }
    $

    template<>
    xstruct(
        xspec(mirror, void)
    ) $
}

#endif

xexport_space(mixc::dumb_mirror::origin)