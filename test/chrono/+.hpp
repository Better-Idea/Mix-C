#ifndef xpack_test_chrono
#define xpack_test_chrono
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_chrono
        #include"chrono/+.hpp"
        #include"define/base_type.hpp"
        #include"macro/xassert.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::test_chrono{
        xtest(chrono){
            using namespace inc;

            for(u32 y = 1, total_day = 0; y < 2048; y++){
                constexpr u16 leap  [] = { 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };
                constexpr u16 normal[] = { 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
                auto first      = date{ y, 1, 1 };
                auto day_begin  = day(first);
                auto lut        = first.is_leap() ? leap : normal;
                auto end        = lut[11];

                for(u32 i = 1, m = 1, d = 1; i <= end; i++, d++, total_day++){
                    if (i > lut[0]){
                        d    = 1;
                        m   += 1;
                        lut += 1;
                    }

                    day  day_sum    = date{ y, m, d };
                    date today      = day_sum;
                    auto diff       = day_sum - day_begin;

                    xassert_eq(total_day, day_sum);
                    xassert_eq(diff, i - 1);
                    xassert_eq(y, today.year());
                    xassert_eq(m, today.month());
                    xassert_eq(d, today.day());
                }
            }
        };
    }
#endif
