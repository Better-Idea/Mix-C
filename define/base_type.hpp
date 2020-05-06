#ifndef xpack_define_base_type
#define xpack_define_base_type
    using i08      = signed char;
    using i16      = signed short;
    using i32      = signed int;
    using i64      = signed long long;
    using u08      = unsigned char;
    using u16      = unsigned short;
    using u32      = unsigned int;
    using u64      = unsigned long long;
    using f32      = float;
    using f64      = double;
    using asciis   = const char *;
    using words    = const char16_t *;
    using ci08     = const i08;
    using ci16     = const i16;
    using ci32     = const i32;
    using ci64     = const i64;
    using cu08     = const u08;
    using cu16     = const u16;
    using cu32     = const u32;
    using cu64     = const u64;
    using cf32     = const f32;
    using cf64     = const f64;
    using i08p     = i08  *;
    using i16p     = i16  *;
    using i32p     = i32  *;
    using i64p     = i64  *;
    using u08p     = u08  *;
    using u16p     = u16  *;
    using u32p     = u32  *;
    using u64p     = u64  *;
    using f32p     = f32  *;
    using f64p     = f64  *;
    using w16p     = char16_t  *;
    using voidp    = void *;
    using byte     = u08;
    
    #ifndef _MSC_BUILD
        using size_t        = long unsigned int;
        #define size_t      size_t
    #endif

    #define the         (*(the_t *)this)
    #define thex        (*(final *)this)

    namespace mixc::base_type{
        template<int> struct env{};
        
        template<>
        struct env<4>{
            using ixx       = i32;
            using uxx       = u32;
        };

        template<>
        struct env<8>{
            using ixx       = i64;
            using uxx       = u64;
        };
    }

    using ixx           = mixc::base_type::env<sizeof(void *)>::ixx;
    using uxx           = mixc::base_type::env<sizeof(void *)>::uxx;
    using ixxp          = ixx *;
    using uxxp          = uxx *;

    enum class bstate{ // binary state
        fail,
        success,
    };

    constexpr uxx       not_exist    = uxx(-1);
    constexpr uxx       magic_number = 19961212;
    constexpr bstate    fail         = bstate::fail;
    constexpr bstate    success      = bstate::success;
    
    enum class ini {};
    constexpr ini ini_now = ini(0);

    struct length {
        explicit length(uxx value) : value(value) { }
        operator uxx () {
            return value;
        }
    private:
        uxx value;
    };

#endif

namespace xuser{
    using namespace ::mixc::base_type;
}
