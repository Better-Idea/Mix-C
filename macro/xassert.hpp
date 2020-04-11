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
                inc::print(" TEST | %-24s", name);
            }

            ~result(){
                if (error_count == 0){
                    inc::print(" [PASS | CASE:%-9u]\n", case_count);
                }
                else{
                    inc::print('\n');
                }
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
            tty.write("\n");                                                                            \
            log(                                                                                        \
                __test.error_count += 1,                                                                \
                __FILE__,                                                                               \
                __LINE__,                                                                               \
                #__VA_ARGS__ ",", ## __VA_ARGS__);                                                      \
            if (__test.error_count % __test.error_threshold == 0) {                                     \
                tty.write("\npause key to continue...");                                                \
                tty.read_key();                                                                         \
                tty.write("\n");                                                                        \
            }                                                                                           \
            return true;                                                                                \
        }                                                                                               \
    })())

    #define xassert_eq(the_wanted,...)                                                                  \
    if (auto const & wanted = (the_wanted), & actually = (__VA_ARGS__); false) {}                       \
    else xassert(wanted == actually, wanted, actually)

    #define xtest(name,...)                                                                             \
    struct name{                                                                                        \
        template<class call>                                                                            \
        name(call && item){                                                                             \
            mixc::macro_xassert::result r(__func__, ## __VA_ARGS__);                                    \
            item(r);                                                                                    \
        }                                                                                               \
    } xlink2(__, __COUNTER__) = [](mixc::macro_xassert::result & __test)
#endif
