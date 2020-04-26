#ifndef xpack_dumb_mirror
#define xpack_dumb_mirror
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::dumb_mirror
        #include"define/base_type.hpp"
        #include"memop/addressof.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_mirror{
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
    }

#endif

namespace xuser::inc{
    using ::mixc::dumb_mirror::mirror;
}
