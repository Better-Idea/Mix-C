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
                inc::print_core(" TEST | %-24s\n", name);
            }

            ~result(){
                asciis msg[] = { "fail", "pass" };
                inc::print_core("      | case:%-9u %s\n", case_count, msg[error_count == 0]);
            }
        };
    }

    #define xassert(condition,...)                                                                      \
    if (([&](){                                                                                         \
        if (__test.case_count += 1; condition){                                                         \
            return false;                                                                               \
        }                                                                                               \
        else{                                                                                           \
            using namespace ::mixc::macro_xassert::inc;                                                 \
            log(                                                                                        \
                __test.error_count += 1,                                                                \
                __FILE__,                                                                               \
                __LINE__,                                                                               \
                #__VA_ARGS__ ",", ## __VA_ARGS__);                                                      \
            if (__test.error_count % __test.error_threshold == 0) {                                     \
                tty.write("pause key to continue...");                                                  \
                tty.read_key();                                                                         \
                tty.write("\n");                                                                        \
            }                                                                                           \
            return true;                                                                                \
        }                                                                                               \
    })())

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
