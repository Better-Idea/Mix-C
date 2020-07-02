#ifndef xpack_dumb_mirror
#define xpack_dumb_mirror
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::dumb_mirror
    #include"mixc.hpp"

    namespace xuser{
        template<class type>
        struct mirror{
            using items_t = u08 [sizeof(type)];
            xgc_fields(
                xiam(mirror<type>),
                xpri(bytes, items_t)
            );
        public:
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
        };

        template<>
        struct mirror<void>{
            xgc_fields(
                xiam(mirror<void>)
            );
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::dumb_mirror::mirror;
}
