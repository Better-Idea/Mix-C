#ifndef xpack_test_math_index_system
#define xpack_test_math_index_system
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_math_index_system
        #include"define/base_type.hpp"
        #include"macro/xassert.hpp"
        #include"simd/x86/sstr.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::test_math_index_system{
        xtest(sstr_index_of_first){
            sstr str;
            str.ptr = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ$";

            for(i32 len = 1; len <= 64; len++){
                str.len = len;
                for(i32 i = 0; i < len; i++){
                    auto acturlly = ::sstr_index_of_first(str, str.ptr[i]);
                    xassert_eq(i, acturlly);
                }
            }
        };
    }
#endif
