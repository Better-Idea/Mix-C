#ifndef xpack_define_nullref_t
#define xpack_define_nullref_t
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::define_nullref_t
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::define_nullref_t{
        xgc(nullref_t)
            xgc_fields();

            template<class a> friend auto operator == (a const & value, nullref_t) {
                return voidp(& value) == nullref_t::value;
            }

            template<class a> friend auto operator == (nullref_t, a const & value) {
                return voidp(& value) == nullref_t::value;
            }

            template<class a> friend auto operator != (a const & value, nullref_t) {
                return voidp(& value) != nullref_t::value;
            }

            template<class a> friend auto operator != (nullref_t, a const & value) {
                return voidp(& value) != nullref_t::value;
            }
        xgc_end();
        
        inline const nullref_t & nullref = *(nullref_t *)nullptr;
    }

#endif

namespace xuser::inc{
    using namespace mixc::define_nullref_t;
}
