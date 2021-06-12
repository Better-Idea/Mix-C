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
    using f80      = long double;
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
    using ci08p    = const i08 *;
    using ci16p    = const i16 *;
    using ci32p    = const i32 *;
    using ci64p    = const i64 *;
    using cu08p    = const u08 *;
    using cu16p    = const u16 *;
    using cu32p    = const u32 *;
    using cu64p    = const u64 *;
    using cf32p    = const f32 *;
    using cf64p    = const f64 *;
    
    #ifndef size_t
        using size_t = decltype(sizeof(0));
        #define size_t      size_t
    #endif

    #define the         (*(the_t *)this)
    #define thex        (*(final_t *)this)

    namespace mixc::base_type{
        template<int> struct env{};

        template<>
        struct env<4>{
            using ixx              = i32;
            using uxx              = u32;
            using thread_counter_t = u16;
        };

        template<>
        struct env<8>{
            using ixx              = i64;
            using uxx              = u64;
            using thread_counter_t = u32;
        };
    }

    struct u128{
        u64 low     = 0;
        u64 high    = 0;

        u128(){}
        u128(u64 low, u64 high) : 
            low(low), high(high){}

        auto & l(u64 value){
            low = value;
            return this[0];
        }

        auto & h(u64 value){
            high = value;
            return this[0];
        }
    };

    template<class type_t>
    struct quo_rem_pair{
        type_t quotient;
        type_t remainder;

        auto & q(type_t value){
            quotient = value;
            return this[0];
        }

        auto & r(type_t value){
            remainder = value;
            return this[0];
        }
    };

    using ixx               = mixc::base_type::env<sizeof(void *)>::ixx;
    using uxx               = mixc::base_type::env<sizeof(void *)>::uxx;
    using thread_counter_t  = mixc::base_type::env<sizeof(void *)>::thread_counter_t;
    using ixxp              = ixx *;
    using uxxp              = uxx *;

    enum class bstate_t{ // binary state
        fail,
        success,
    };

    enum class loop_t{
        finish,
        go_on,
    };

    struct zero_t{
        constexpr zero_t(){}

        operator uxx() const {
            return 0;
        }
    };

    constexpr uxx       not_exist    = uxx(-1);
    constexpr uxx       magic_number = 19961212;
    constexpr zero_t    zero{};

    struct length {
        explicit length(uxx value) : value(value) { }
        operator uxx () {
            return value;
        }
    private:
        uxx value;
    };

    #if !__GNUC__
    #define not     !
    #define and     && 
    #define or      ||
    #endif
#endif
