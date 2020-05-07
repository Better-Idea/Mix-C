#ifndef xpack_test_math_index_system
#define xpack_test_math_index_system
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_math_index_system
        #include"define/base_type.hpp"
        #include"macro/xassert.hpp"
        #include"simd/x86/sstr.hpp"
        #include"string.h"
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
