#ifndef xpack_define_nullref_t
#define xpack_define_nullref_t
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::define_nullref_t
        #include"define/base_type.hpp"
        #include"memop/addressof.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::define_nullref_t{
        struct nullref_t{
            xgc_fields(
                xiam(nullref_t)
            );
        public:
            template<class a> friend auto operator == (a const & value, nullref_t) {
                return inc::addressof(value) == nullptr;
            }

            template<class a> friend auto operator == (nullref_t, a const & value) {
                return inc::addressof(value) == nullptr;
            }

            template<class a> friend auto operator != (a const & value, nullref_t) {
                return inc::addressof(value) != nullptr;
            }

            template<class a> friend auto operator != (nullref_t, a const & value) {
                return inc::addressof(value) != nullptr;
            }

            template<class a>
            operator a & () const {
                return *(a *)this;
            }
        };

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wnull-dereference"
        inline const nullref_t & nullref = *(nullref_t *)nullptr;
        #pragma GCC diagnostic pop
    }

#endif

namespace xuser::inc{
    using ::mixc::define_nullref_t::nullref;
}
