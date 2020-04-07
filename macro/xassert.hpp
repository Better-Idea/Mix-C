#pragma once
#include"macro/private/mix.hpp"
#include"macro/xlink.hpp"

namespace mixc::macro_xassert{
    struct result{
        u32 error_threshold;
        u32 error_count     = 0;
        u32 case_count      = 0;

        result(asciis name, uxx error_threshold = 128) : 
            error_threshold(error_threshold){
            printf(" TEST | %-24s", name);
        }

        ~result(){
            if (error_count == 0){
                printf(" [PASS | CASE:%-9u]\n", case_count);
            }
            else{
                putchar('\n');
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
        putchar('\n');                                                                              \
        mixc::macro_private_mix::mix::print(                                                        \
            __test.error_count += 1,                                                                \
            __FILE__,                                                                               \
            __LINE__,                                                                               \
            #__VA_ARGS__ ",", ## __VA_ARGS__);                                                      \
        if (__test.error_count % __test.error_threshold == 0) {                                     \
            printf("\npause key to continue...");                                                   \
            getchar();                                                                              \
            putchar('\n');                                                                          \
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
