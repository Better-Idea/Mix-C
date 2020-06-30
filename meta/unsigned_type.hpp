#ifndef xpack_meta_unsigned_type
#define xpack_meta_unsigned_type
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::meta_unsigned_type
    #include"define/base_type.hpp"

    namespace xuser{
        template<uxx>
        struct meta{};
        template<> struct meta<1> { using result = u08; };
        template<> struct meta<2> { using result = u16; };
        template<> struct meta<4> { using result = u32; };
        template<> struct meta<8> { using result = u64; };

        template<class type>
        using unsigned_type = typename meta<sizeof(type)>::result;
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::meta_unsigned_type::unsigned_type;
}
