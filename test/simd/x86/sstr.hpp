#ifndef xpack_test_math_index_system
#define xpack_test_math_index_system
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_math_index_system
        // std first
        #include<string.h>
        #include<stdlib.h>

        #include"define/base_type.hpp"
        #include"io/tty.hpp"
        #include"lang/cxx/compare.hpp"
        #include"lang/cxx/strlize.hpp"
        #include"lang/cxx.hpp"
        #include"lang/wxx/to_lower.hpp"
        #include"lang/wxx/to_upper.hpp"
        #include"lang/wxx.hpp"
        #include"macro/xassert.hpp"
        #include"math/random.hpp"
        #include"simd/x86/sstr.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::test_math_index_system{
        xtest(sstr_index_of_first){
            sstr str;
            str.ptr = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$";

            for(str.len = 0; str.len < 64; str.len++){
                for(i32 i = 0; i < str.len; i++){
                    auto acturlly = ::sstr_index_of_first(str, str.ptr[i]);
                    xassert_eq(i, acturlly);
                }
                auto acturlly = ::sstr_index_of_first(str, ' ');
                xassert(acturlly == not_exist, acturlly, str.len);
            }
        };

        xtest(sstr_index_of_last){
            sstr str;
            char buf[65] = "0000000000000000000000000000000000000000000000000000000000000000";
            str.ptr      = buf;

            for(str.len = 0; str.len < 64; str.len++){
                for(i32 i = 0; i < str.len; i++){
                    buf[i] = '1';
                    auto acturlly = ::sstr_index_of_last(str, '1');
                    xassert(i == acturlly, i, acturlly, str.len);
                    buf[i] = '0';
                }
                auto acturlly = ::sstr_index_of_last(str, '1');
                xassert(acturlly == not_exist, acturlly, str.len);
            }
        };

        xtest(sstr_stou){
            using namespace inc;
            char buf[16];
            sstr str;
            u32  actually;
            u32  wanted;
            str.ptr = buf;

            for(wanted = 0; wanted < 256; wanted++){
                str.len = c08(wanted, [&](uxx length){
                    buf[length] = 0;
                    return buf;
                }).length();

                actually = ::sstr_stou(str);
                xassert_eq(wanted, actually);
            }

            #define xgen(value)                         \
            str.ptr = #value;                           \
            str.len = sizeof(#value) - 1;               \
            actually = ::sstr_stou(str);                \
            wanted   = value;                           \
            xassert_eq(wanted, actually)

            xgen(685);
            xgen(1234);
            xgen(23456);
            xgen(345678);
            xgen(4567890);
            xgen(12345678);
            xgen(123456789);
            xgen(4294967294);
            xgen(4294967295);
            #undef  xgen

            for(int i = 0; i < 32768; i++){
                u32 width = random<u32>() % 9 + 1;
                for(int ii = 0; ii < width; ii++){
                    buf[ii] = "0123456789"[random<u08>() % 10];
                }
                buf[width] = 0;
                str.ptr    = buf;
                str.len    = width;

                wanted     = ::atoi(buf);
                actually   = ::sstr_stou(str);
                xassert_eq(wanted, actually);
            }
        };

        xtest(sstr_to_lower) {
            using namespace inc;
            char abuf[] = " `1234567890-=[]\\;',./~!@#$%^&*()_+{}|:\"<>?abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            char bbuf[sizeof(abuf)];
            bool test;
            char t;
            sstr a;
            sstr b;
            sstr c;
            c08  actually;
            c08  wanted;
            b.ptr = bbuf;
            
            #define xgen(wanna,value)                   \
            wanted   = wanna;                           \
            a.ptr    = value;                           \
            a.len    = sizeof(value) - 1;               \
            bbuf[a.len] = 0;                            \
            b        = ::sstr_to_lower(b, a);           \
            actually = c08(b.ptr, b.len);               \
            xassert(wanted.compare(actually) == 0, wanted, actually)

            xgen("a", "A");
            xgen("ab", "AB");
            xgen("abz", "ABZ");
            xgen("abz{", "ABZ{");
            xgen("@abz{", "@ABZ{");
            #undef  xgen

            a.ptr = abuf;
            a.len = sizeof(abuf) - 1;
            b.ptr = bbuf;
            b.len = sizeof(bbuf);

            for(uxx i = 1; i < sizeof(abuf) - 1; i++){
                a.len           = i;
                c               = ::sstr_to_lower(b, a);
                t               = wanted[c.len];
                wanted          = c08{ a.ptr, a.len };
                wanted[c.len]   = 0;
                actually        = c08{ c.ptr, c.len };
                actually[a.len] = 0;
                test            = wanted.compare(actually, []xcmp(char){
                    return w08(left).to_lower() - right;
                }) == 0;
                xassert(test, wanted, actually, i);
                wanted[c.len]   = t;
            }
        };

        xtest(sstr_to_upper) {
            using namespace inc;
            char abuf[] = " `1234567890-=[]\\;',./~!@#$%^&*()_+{}|:\"<>?abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
            char bbuf[sizeof(abuf)];
            bool test;
            char t;
            sstr a;
            sstr b;
            sstr c;
            c08  actually;
            c08  wanted;
            b.ptr = bbuf;
            
            #define xgen(wanna,value)                   \
            wanted   = wanna;                           \
            a.ptr    = value;                           \
            a.len    = sizeof(value) - 1;               \
            bbuf[a.len] = 0;                            \
            b        = ::sstr_to_upper(b, a);           \
            actually = c08(b.ptr, b.len);               \
            xassert(wanted.compare(actually) == 0, wanted, actually)

            xgen("A", "a");
            xgen("AB", "ab");
            xgen("ABZ", "abz");
            xgen("ABZ{", "abz{");
            xgen("@ABZ{", "@abz{");
            #undef  xgen

            a.ptr = abuf;
            a.len = sizeof(abuf) - 1;
            b.ptr = bbuf;
            b.len = sizeof(bbuf);

            for(uxx i = 1; i < sizeof(abuf) - 1; i++){
                a.len           = i;
                c               = ::sstr_to_upper(b, a);
                t               = wanted[c.len];
                wanted          = c08{ a.ptr, a.len };
                wanted[c.len]   = 0;
                actually        = c08{ c.ptr, c.len };
                actually[a.len] = 0;
                test            = wanted.compare(actually, []xcmp(char){
                    return w08(left).to_upper() - right;
                }) == 0;
                xassert(test, wanted, actually, i);
                wanted[c.len]   = t;
            }
        };

        xtest(sstr_compare){
            char abuf[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$ ";
            char bbuf[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$ ";
            sstr a;
            sstr b;
            ixx  actually;
            ixx  wanted;
            a.ptr = abuf;
            b.ptr = bbuf;

            a.len    = 0;
            abuf[0]   = b.ptr[0];
            b.len    = 0;
            actually = ::sstr_compare(a, b);
            xassert(actually == 0);

            a.len    = 1;
            actually = ::sstr_compare(a, b);
            xassert(actually > 0);

            actually = ::sstr_compare(b, a);
            xassert(actually < 0);

            auto test = [&](i32 i){
                char at;
                char bt;
                actually     = ::sstr_compare(a, b);
                at           = abuf[a.len];
                bt           = bbuf[b.len];
                abuf[a.len]  = 0;
                bbuf[b.len]  = 0;
                wanted       = ::strcmp(a.ptr, b.ptr);
                xassert((actually ^ wanted) >= 0, wanted, actually, i, a.len, b.len, a.ptr, b.ptr);
                abuf[a.len]  = at;
                bbuf[b.len]  = bt;
            };

            for(a.len = 1; a.len <= 64; a.len++){
                for(b.len = 1; b.len <= 64; b.len++){
                    for(i32 i = 0; i < b.len; i++){
                        abuf[i] += 1; // grater
                        test(i);

                        abuf[i] -= 1; // equal
                        test(i);

                        abuf[i] -= 1; // less
                        test(i);
                        abuf[i] += 1;
                    }
                }
            }
        };
    }
#endif
