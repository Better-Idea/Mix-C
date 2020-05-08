#ifndef xpack_test_math_index_system
#define xpack_test_math_index_system
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_math_index_system
        #include"define/base_type.hpp"
        #include"docker/array.hpp"
        #include"macro/xassert.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::test_math_index_system{
        xtest(index_system){
            using namespace inc;
            array<char, 4> seq;
            auto s = seq.range(oo{0, -1});

            xassert_eq(5, s.length());
            xassert_eq(& seq[0], & s[0]);
            xassert_eq(& seq[1], & s[1]);
            xassert_eq(& seq[2], & s[2]);
            xassert_eq(& seq[3], & s[3]);
            xassert_eq(& seq[4], & s[4]); // over range allowed

            s = seq.range(oo{-1, 0});
            xassert_eq(& seq[4], & s[0]); // over range allowed
            xassert_eq(& seq[3], & s[1]);
            xassert_eq(& seq[2], & s[2]);
            xassert_eq(& seq[1], & s[3]);
            xassert_eq(& seq[0], & s[4]);

            s = seq.range(oc{0});
            xassert_eq(4, s.length());
            xassert_eq(& seq[0], & s[0]);
            xassert_eq(& seq[1], & s[1]);
            xassert_eq(& seq[2], & s[2]);
            xassert_eq(& seq[3], & s[3]);

        };
    }
#endif
