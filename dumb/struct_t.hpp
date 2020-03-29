#ifndef xpack_dumb_struct_t
#define xpack_dumb_struct_t
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::dumb_struct_t
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
        #include"meta/is_class.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_struct_t{
        template<class type, bool is_class> struct struct_t;
        template<class type>
        xgcx(struct_t, xtmpl(type, true), xpub(type))
            xgc_fields();
            using type::type;
        xgc_end();

        template<class type>
        xgcx(struct_t, xtmpl(type, false))
            xgc_fields(
                xpro(data, type)
            );

            template<class ... args>
            struct_t(args const & ... list) : 
                data(list...){
            }

            operator type & (){
                return data;
            }

            operator const type & () const {
                return data;
            }
        xgc_end();
    }

#endif

namespace xuser::inc{
    template<class type>
    using struct_t = ::mixc::dumb_struct_t::
        struct_t<type, mixc::dumb_struct_t::inc::is_class<type>>;
}
