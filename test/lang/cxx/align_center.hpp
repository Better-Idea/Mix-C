#ifndef xpack_test_lang_cxx_align_center
#define xpack_test_lang_cxx_align_center
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_lang_cxx_align_center
        #include"define/base_type.hpp"
        #include"lang/cxx/align_center.hpp"
        #include"lang/cxx/compare.hpp"
        #include"lang/cxx.hpp"
        #include"macro/xassert.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::test_lang_cxx_align_center{
        xtest(align_center){
            inc::c08 w = "";
            inc::c08 a;
            inc::c08 b;
            auto && alloc = [](uxx length){
                static char buf[100];
                buf[length] = '\0';
                return buf;
            };
            
            b = w.align_center(0, alloc);
            xassert_eq(0, b.length());

            a = "          ";
            b = w.align_center(10, alloc);
            xassert_eq(0, a.compare(b));

            w = "h";
            a = "h";
            b = w.align_center(1, alloc);
            xassert_eq(0, a.compare(b));

            a = "h ";
            b = w.align_center(2, alloc);
            xassert_eq(0, a.compare(b));

            a = " h ";
            b = w.align_center(3, alloc);
            xassert_eq(0, a.compare(b));

            a = " h  ";
            b = w.align_center(4, alloc);
            xassert_eq(0, a.compare(b));

            w = "hh";
            a = "h";
            b = w.align_center(1, alloc);
            xassert_eq(0, a.compare(b));

            a = "hh";
            b = w.align_center(2, alloc);
            xassert_eq(0, a.compare(b));

            a = "hh ";
            b = w.align_center(3, alloc);
            xassert_eq(0, a.compare(b));

            a = " hh ";
            b = w.align_center(4, alloc);
            xassert_eq(0, a.compare(b));
            
            a = " hh  ";
            b = w.align_center(5, alloc);
            xassert_eq(0, a.compare(b));
        };
    }

#endif
