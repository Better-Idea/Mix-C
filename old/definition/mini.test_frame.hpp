#pragma once
#ifdef xenable_test_frame
    #include<stdio.h>
    #define private public
    #define xmark_test                                              \
    struct xline(__test_struct_) {                                  \
        static bool & state(){                                      \
            static bool state = need_test;                          \
            return state;                                           \
        }                                                           \
        ~xline(__test_struct_) () {                                 \
            if (state() == need_test && mixc_test_can_show()){      \
                printf("+ %-5d| line %5d | " __FILE__ "\n",         \
                ++global_miss_test_count, __LINE__);                \
            }                                                       \
        }                                                           \
    } xline(__test_target_);

    #define xtest_can_show                                          \
        static bool & mixc_test_can_show() {                        \
            static bool state = false;                              \
            return state;                                           \
        }

    #define xtested     xline(__test_struct_)::state() = tested;
    #define xtest_reflect                                           \
        template<class a>                                           \
        void mixc_destructors(a & self){                            \
            self.~a();                                              \
        }                                                           \
        void mixc_test_reflect() {                                  \
            thes.mixc_test_can_show() = true;                       \
            thei.mixc_test_can_show() = true;                       \
            thes.mixc_destructors(thei);                            \
            thes.mixc_destructors(thes);                            \
            thes.mixc_test_can_show() = false;                      \
            thei.mixc_test_can_show() = false;                      \
        }                                                           \
        xtest_can_show
    
    #define xtest_unit(name,...)                                    \
    struct name {                                                   \
    private:                                                        \
        static bool & __test_when_free__(){                         \
            static bool state = false;                              \
            return state;                                           \
        }                                                           \
        template<class a>                                           \
        static void which_not_test(a const & first){                \
            ((a &)first).mixc_test_reflect();                       \
        }                                                           \
        template<class a, class b, class ... args>                  \
        static void which_not_test(                                 \
            a const & first,                                        \
            b const & second,                                       \
            args const & ... list){                                 \
            which_not_test(first);                                  \
            which_not_test(second, list...);                        \
        }                                                           \
    public:                                                         \
        static void test_all(){                                     \
            char header[39] = { 0 };                                \
            for (int i = 0;                                         \
                i < sizeof(header) - sizeof(#name) - 1; i++){       \
                header[i] = '=';                                    \
            }                                                       \
            printf("%s %s unit\n", header, #name);                  \
            __test_when_free__() = true; {                          \
                name ins;                                           \
            }                                                       \
            __test_when_free__() = false;                           \
            printf("\n");                                           \
            which_not_test(__VA_ARGS__);                            \
        }                                                           \

    #define xtest_item(name,...)                                    \
        struct xline(__test_struct_) {                              \
            ~xline(__test_struct_) () {                             \
                if (__test_when_free__()){                          \
                    name();                                         \
                }                                                   \
            }                                                       \
        } xline(__test_target_);                                    \
        static void name() {                                        \
            struct __inner__{                                       \
                __inner__() : fail(false), use_case(0) {            \
                    auto a = "" #__VA_ARGS__;                       \
                    auto b = #name;                                 \
                    printf("%-37s", a[0] ? a : b);                  \
                }                                                   \
                ~__inner__(){                                       \
                    if (fail == false){                             \
                        printf("[PASS]\n");                         \
                    }                                               \
                    else{                                           \
                        printf("+ + + + + + + + + + + "             \
                            "+ + + + + + + + + + +\n");             \
                    }                                               \
                }                                                   \
                bool fail;                                          \
                i32  use_case;                                      \
            } __test__;
        
    #define xassert(condition,...)                                  \
        __test__.use_case += 1;                                     \
        if ([&](){                                                  \
            if (condition){                                         \
                return true;                                        \
            }                                                       \
            if (__test__.fail == false){                            \
                printf("[FAIL]\n");                                 \
            }                                                       \
            __test__.fail = true;                                   \
            printf("+ %-.4d | line %-.4d | case %-.4d\n",           \
                ++global_test_error_count,                          \
                __test__.use_case,                                  \
                __LINE__                                            \
            );                                                      \
            mixc::mini_test_frame_private::error(__VA_ARGS__);      \
            return false;                                           \
        }() == false)
    
    xspace(mixc)
        xspace(mini_test_frame_private)
            template<class ... args> xdef(error, asciis fmt, args... list)
                printf("+      | ");
                printf(fmt, list...);
                printf("\n");
            $

            xdef(error) $
        $
    $

#else
    #define xmark_test
    #define xtested
    #define xtest_reflect
    #define xtest_unit(name)
    #define xtest_item(name)
    #define xassert(condition)
#endif
