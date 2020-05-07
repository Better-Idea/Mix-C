#ifndef xpack_test_lang_cxx_compare
#define xpack_test_lang_cxx_compare
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_lang_cxx_compare
        #include"define/base_type.hpp"
        #include"lang/cxx/compare.hpp"
        #include"lang/cxx.hpp"
        #include"macro/xassert.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::test_lang_cxx_compare{
        xtest(compare){
            inc::c08 a = "";
            inc::c08 b = "";

            xassert_eq(0, a.compare(b));

            a = "1";
            xassert_eq(ixx(1) , a.compare(b));
            xassert_eq(ixx(-1), b.compare(a));

            a = "1";
            b = "0";
            xassert_eq(ixx(1), a.compare(b));

            a = "1";
            b = "1";
            xassert_eq(0, a.compare(b));

            a = "1";
            b = "2";
            xassert_eq(ixx(-1), a.compare(b));

            a = "1";
            b = "12";
            xassert_eq(ixx(-1), a.compare(b));
            xassert_eq(ixx(1) , b.compare(a));

            a = "2";
            b = "12";
            xassert_eq(ixx(1) , a.compare(b));
            xassert_eq(ixx(-1), b.compare(a));
        };
    }
#endif