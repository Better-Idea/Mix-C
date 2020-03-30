#ifndef xpack_meta_unsigned_type
#define xpack_meta_unsigned_type
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_unsigned_type
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_unsigned_type{
        template<uxx>
        struct unsigned_type{};
        template<> struct unsigned_type<1> { using result = u08; };
        template<> struct unsigned_type<2> { using result = u16; };
        template<> struct unsigned_type<4> { using result = u32; };
        template<> struct unsigned_type<8> { using result = u64; };
    }

#endif

namespace xuser::inc{
    template<class type>
    using unsigned_type = 
        typename ::mixc::meta_unsigned_type::unsigned_type<sizeof(type)>::result;
}
