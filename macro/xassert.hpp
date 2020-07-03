#ifndef xpack_macro_xassert
#define xpack_macro_xassert
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_xassert
    #include"define/base_type.hpp"
    #include"io/tty.hpp"
    #include"io/private/tty.hpp"
    #include"macro/private/log.hpp"
    #include"macro/xlink.hpp"
    #pragma pop_macro("xuser")
    
    namespace mixc::macro_xassert{
        struct result{
            u32 error_threshold;
            u32 error_count     = 0;
            u32 case_count      = 0;

            result(asciis name, uxx error_threshold = 128) : 
                error_threshold(error_threshold){
                inc::tty.write_line("TEST | ", name);
            }

            ~result(){
                asciis msg[] = { "fail", "pass" };
                inc::tty.write_line("     | case:", case_count, " ", msg[error_count == 0]);
            }
        };
    }

    #define xassert(condition,...)                                                                      \
    if (([&](asciis func){                                                                              \
        if (__test.case_count += 1; condition){                                                         \
            return false;                                                                               \
        }                                                                                               \
        else{                                                                                           \
            using namespace mixc::macro_private_log::origin;                                            \
            using namespace mixc::io_tty::origin;                                                       \
            __test.error_count += 1;                                                                    \
            log(                                                                                        \
                for_test,                                                                               \
                __FILE__,                                                                               \
                __LINE__,                                                                               \
                func,                                                                                   \
                "error, " #__VA_ARGS__ ",", __test.error_count, ## __VA_ARGS__);                        \
            if (__test.error_count % __test.error_threshold == 0) {                                     \
                tty.write("pause key to continue...");                                                  \
                tty.flush();                                                                            \
                tty.read_key();                                                                         \
                tty.write("\n");                                                                        \
            }                                                                                           \
            return true;                                                                                \
        }                                                                                               \
    })(__func__))

    #define xassert_eq(...)                                                                             \
    if (([&](auto const & wanted, auto const & actually){                                               \
        xassert(wanted == actually, wanted, actually) return true;                                      \
        return false;                                                                                   \
    })(__VA_ARGS__))

    #define xtest(name,...)                                                                             \
    struct name{                                                                                        \
        template<class call>                                                                            \
        name(call && item){                                                                             \
            ::mixc::macro_xassert::result r(__func__, ## __VA_ARGS__);                                  \
            item(r);                                                                                    \
        }                                                                                               \
    } xlink2(__, __COUNTER__) = [](::mixc::macro_xassert::result & __test)

#endif
