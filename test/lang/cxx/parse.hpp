#ifndef xpack_test_lang_cxx_parse
#define xpack_test_lang_cxx_parse
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_lang_cxx_parse
        #include"define/base_type.hpp"
        #include"lang/cxx/parse.hpp"
        #include"lang/cxx.hpp"
        #include"docker/array.hpp"
        #include"macro/xassert.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::test_lang_cxx_parse{
        xtest(parse){
            inc::c08 w = "2020";
            union{
                u32 u = 2020;
                i32 i;
                u64 ux;
                i64 ix;
            };
            auto b = w.parse<u32>();
            xassert_eq(u        , u32(b));
            xassert_eq(false    , b.is_parse_error());
            xassert_eq(not_exist, b.index_of_error());

            struct pair{ inc::c08 str; u64 v; };
            auto && v = inc::array_view {
                pair { "0" , 0  },
                pair { "1" , 1  },
                pair { "2" , 2  },
                pair { "3" , 3  },
                pair { "4" , 4  },
                pair { "5" , 5  },
                pair { "6" , 6  },
                pair { "7" , 7  },
                pair { "8" , 8  },
                pair { "9" , 9  },
                pair { "00", 0  },
                pair { "01", 1  },
                pair { "02", 2  },
                pair { "03", 3  },
                pair { "04", 4  },
                pair { "05", 5  },
                pair { "06", 6  },
                pair { "07", 7  },
                pair { "08", 8  },
                pair { "09", 9  },
                pair { "10", 10 },
                pair { "11", 11 },
                pair { "12", 12 },
                pair { "13", 13 },
                pair { "14", 14 },
                pair { "15", 15 },
                pair { "16", 16 },
                pair { "17", 17 },
                pair { "18", 18 },
                pair { "19", 19 },
                pair { "123", 123 },
                pair { "1234", 1234 },
                pair { "12345", 12345 },
                pair { "123456", 123456 },
                pair { "1234567", 1234567 },
                pair { "12345678", 12345678 },
                pair { "123456789", 123456789 },
                pair { "4294967293", 4294967293 },
                pair { "4294967294", 4294967294 },
                pair { "4294967295", 4294967295 },

                pair { "+4294967295", 4294967295 },
                pair { "+4294967294", 4294967294 },
                pair { "+4294967293", 4294967293 },
                pair { "+123456789", 123456789 },
                pair { "+12345678", 12345678 },
                pair { "+1234567", 1234567 },
                pair { "+123456", 123456 },
                pair { "+12345", 12345 },
                pair { "+1234", 1234 },
                pair { "+123", 123 },
                pair { "+19", 19 },
                pair { "+18", 18 },
                pair { "+17", 17 },
                pair { "+16", 16 },
                pair { "+15", 15 },
                pair { "+14", 14 },
                pair { "+13", 13 },
                pair { "+12", 12 },
                pair { "+11", 11 },
                pair { "+10", 10 },
                pair { "+09", 9  },
                pair { "+08", 8  },
                pair { "+07", 7  },
                pair { "+06", 6  },
                pair { "+05", 5  },
                pair { "+04", 4  },
                pair { "+03", 3  },
                pair { "+02", 2  },
                pair { "+01", 1  },
                pair { "+00", 0  },
                pair { "+9" , 9  },
                pair { "+8" , 8  },
                pair { "+7" , 7  },
                pair { "+6" , 6  },
                pair { "+5" , 5  },
                pair { "+4" , 4  },
                pair { "+3" , 3  },
                pair { "+2" , 2  },
                pair { "+1" , 1  },
                pair { "+0" , 0  },
            };

            for(uxx i = 0; i < v.length(); i++){
                u = v[i].v;
                b = v[i].str.parse<u32>();
                xassert_eq(u        , u32(b));
                xassert_eq(false    , b.is_parse_error());
                xassert_eq(not_exist, b.index_of_error());
            }

            w = "-1";
            i = -1;
            b = w.parse<i32>();
            xassert_eq(i        , i32(b));
            xassert_eq(false    , b.is_parse_error());
            xassert_eq(not_exist, b.index_of_error());

            w = "-2147483648";
            i = -2147483648;
            b = w.parse<i32>();
            xassert_eq(i        , i32(b));
            xassert_eq(false    , b.is_parse_error());
            xassert_eq(not_exist, b.index_of_error());

            w      = "-9223372036854775808";
            ix     = -9223372036854775807LL - 1;
            auto c = w.parse<i64>();
            xassert_eq(ix       , i64(c));
            xassert_eq(false    , b.is_parse_error());
            xassert_eq(not_exist, b.index_of_error());
        };
    }
#endif