#ifndef xpack_test_lang_cxx_parse_json
#define xpack_test_lang_cxx_parse_json
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::test_lang_cxx_parse_json
        #include"define/base_type.hpp"
        #include"lang/cxx/compare_fastly.hpp"
        #include"lang/cxx/parse_json.hpp"
        #include"lang/cxx.hpp"
        #include"macro/xassert.hpp"
        #include"macro/xdebug.hpp"
        #include"macro/xdebug_fail.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::test_lang_cxx_parse_json{
        xtest(parse_json){
            auto && alloc = [](uxx length){
                static byte buf[1024 * 32];
                xdebug(true, length);
                xdebug_fail(length > sizeof(buf));
                return buf;
            };
            // char js[] = xjson(
            //     ["table", "class:MyTable", "border:1px solid red", "background-color:yellow",
            //         ["tr",
            //             ["td", "class:MyTd", "border:1px solid black", "text:#5D28D1"],
            //             ["td", "class:MyTd", "border:1px solid black", "text:#5D28D1"]
            //         ],
            //         ["tr",
            //             ["td", "class:MyTd", "border:1px solid black", "text:#5D28D1"],
            //             ["td", "class:MyTd", "border:1px solid black", "text:#5D28D1"]
            //         ],
            //         ["tr",
            //             ["td", "class:MyTd", "border:1px solid black", "text:#5D28D1"],
            //             ["td", "class:MyTd", "border:1px solid black", "text:#5D28D1"]
            //         ]
            //     ]
            // );
            

            inc::c08 x = xjson();
            inc::c08 str;
            f64      real;
            auto     json = x.parse_json(alloc);
            auto     temp = json;
            auto     item = json;
            xassert_eq(0, json.length());

            x    = xjson({});
            json = x.parse_json(alloc);
            xassert_eq(0, json.length());
            
            x    = xjson([]);
            json = x.parse_json(alloc);
            xassert_eq(0, json.length());

            x    = xjson({
                "name" : "PowerfulCat",
                "pwd" : "********",
                "age" : 24,
                "skill" : [
                    "c/c++",
                    "c#",
                    "os",
                    "math",
                    "deep thinking",
                ],
                "circumference_iii" : [
                    106,
                    73,
                    112
                ],
                "home" : "earth",
                "crop" : "iv.galax.solar_system.earth",
                "advance_mvp" : true
            });

            json  = x.parse_json(alloc);
            str   = temp = json["name"];
            xassert_eq(0, str.compare_fastly("PowerfulCat"));
            xassert_eq(inc::json_type::json_string, temp.type());

            str   = temp = json["pwd"];
            xassert_eq(0, str.compare_fastly("********"));
            xassert_eq(inc::json_type::json_string, temp.type());

            real  = temp = json["age"];
            xassert_eq(24.0, real);
            xassert_eq(inc::json_type::json_number, temp.type());

            temp  = json["skill"];
            xassert_eq(inc::json_type::json_array, temp.type());
            xassert_eq(5, temp.length());

            item  = temp[0];
            str   = item;
            xassert_eq(0, str.compare_fastly("c/c++"));
            
            item  = temp[1];
            str   = item;
            xassert_eq(0, str.compare_fastly("c#"));
            
            item  = temp[2];
            str   = item;
            xassert_eq(0, str.compare_fastly("os"));
            
            item  = temp[3];
            str   = item;
            xassert_eq(0, str.compare_fastly("math"));
            
            item  = temp[4];
            str   = item;
            xassert_eq(0, str.compare_fastly("deep thinking"));

            x     = xjson({
                "import" : [
                    "mixc/style.css",
                    "mixc/action.js"
                ],
                "div" : {
                    "class" : ["box", "red"],
                    "size" : [ "100%", "20px" ],
                    "backcolor" : "skyblue",
                    "left-margin" : "10px",
                    "clicked" : "clicked_div",
                    "a" : { "text":"github", "url":"https://github.com/" },
                    "a" : [ "github", "https://github.com/" ]
                },

                "h1" : { "class" : "red", "text" : "json is good choice !" },
                "p" : [ "hello", { "b" : "world" }, "666" ],
                "deimport" : [
                    "mixc/action.js"
                ]
            });

            json  = x.parse_json(alloc);
            
        };
    }
#endif

