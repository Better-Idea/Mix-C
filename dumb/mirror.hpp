#ifndef xpack_dumb_mirror
#define xpack_dumb_mirror
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::dumb_mirror
    #include"mixc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_mirror{
        template<class type>
        using item_t = u08 [sizeof(type)];

        template<class type>
        xstruct(
            xtmpl(mirror, type),
            xprif(bytes, item_t<type>)
        )
            mirror(): bytes{0}{}
            mirror(type const & value){
                this[0] = *(mirror<type> *)(xref value);
            }

            operator type & (){
                return *(type *)bytes;
            }

            constexpr uxx length() const {
                return sizeof(type);
            }
        $

        template<>
        xstruct(
            xspec(mirror, void)
        ) $
    }

#endif

namespace xuser::inc{
    using ::mixc::dumb_mirror::mirror;
}
