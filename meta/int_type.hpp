#ifndef xpack_meta_int_type
#define xpack_meta_int_type
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_int_type
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_int_type{
        template<uxx, bool is_signed>
        struct int_type{};
        template<> struct int_type<1, false> { using result = u08; };
        template<> struct int_type<2, false> { using result = u16; };
        template<> struct int_type<4, false> { using result = u32; };
        template<> struct int_type<8, false> { using result = u64; };
        template<> struct int_type<1, true>  { using result = i08; };
        template<> struct int_type<2, true>  { using result = i16; };
        template<> struct int_type<4, true>  { using result = i32; };
        template<> struct int_type<8, true>  { using result = i64; };
    }

#endif

namespace xuser::inc{
    template<class type>
    using int_type = 
        typename ::mixc::meta_int_type::int_type<sizeof(type), (type(-1) < 0)>::result;
}
