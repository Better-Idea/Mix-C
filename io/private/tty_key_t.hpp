#ifndef xpack_io_private_tty_key_t
#define xpack_io_private_tty_key_t
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::io_private_tty_key_t
        #include"algo/binary_search.hpp"
        #include"define/base_type.hpp"
        #include"docker/array.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xprop.hpp"
        #include"memop/bit.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::io_private{
        namespace func_key{
            enum : u08 {
                esc     = 0x80,
                f1,
                f2,
                f3,
                f4,
                f5,
                f6,
                f7,
                f8,
                top,
                buttom,
                right,
                left,
                end,
                home,
                f9,
                ins,
                del,
                page_up,
                page_down,
                backspace,
            };
        }

        enum key_type : u08{
            is_ascii   = 0x1,
            is_func    = 0x2,
            has_ctrl   = 0x4,
            has_alt    = 0x8,
        };
    }

    namespace mixc::io_private_tty_key_t{
        using namespace mixc::io_private;
        using namespace mixc::io_private::func_key;

        struct pair{ u08 v; asciis name; };

        inline auto const & func_map = inc::array_view {
            pair { esc       , "escape"    },
            pair { ins       , "insert"    },
            pair { del       , "delele"    },
            pair { page_up   , "page_up"   },
            pair { page_down , "page_down" },
            pair { left      , "left"      },
            pair { top       , "top"       },
            pair { buttom    , "buttom"    },
            pair { right     , "right"     },
            pair { end       , "end"       },
            pair { home      , "home"      },
            pair { f1        , "f1"        },
            pair { f2        , "f2"        },
            pair { f3        , "f3"        },
            pair { f4        , "f4"        },
            pair { f5        , "f5"        },
            pair { f6        , "f6"        },
            pair { f7        , "f7"        },
            pair { f8        , "f8"        },
            pair { f9        , "f9"        },
            pair { backspace , "backspace" },
        };


        struct tty_key_t{
            static constexpr uxx max_length = 6;
            using items = u08[max_length];
            using final = tty_key_t;
            using the_t = tty_key_t;

            xgc_fields(
                xthe(tty_key_t),
                xpri(type,   key_type);
                xpri(values, items);
                xpri(count,  uxx);
            );

            tty_key_t() :
                type{}, values{}, count{}{}

            xpubget_pubset(is_ascii, bool){
                xr { return inc::bit(the.type, key_type::is_ascii); }
                xw { inc::bit(& the.type, value, key_type::is_ascii); }
            };

            xpubget_pubset(is_func, bool){
                xr { return inc::bit(the.type, key_type::is_func); }
                xw { inc::bit(& the.type, value, key_type::is_func); }
            };

            xpubget_pubset(has_ctrl, bool){
                xr { return inc::bit(the.type, key_type::has_ctrl); }
                xw { inc::bit(& the.type, value, key_type::has_ctrl); }
            };

            xpubget_pubset(has_alt, bool){
                xr { return inc::bit(the.type, key_type::has_alt); }
                xw { inc::bit(& the.type, value, key_type::has_alt); }
            };

            xpubget_pubset(length, uxx){
                xr { return the.count; }
                xw { the.count = value; }
            };

            xpubget_pubset(value, u08){
                xr { return the.values[0]; }
                xw { the.values[0] = value; }
            };

            xpubget_pubset(ascii, char){
                xr { return the.values[0]; }
                xw { the.values[0] = value; }
            };

            // 临时设施 ============================================
            xpubget(func_name, asciis){
                for(uxx i = 0; i < func_map.length(); i++){
                    if (auto & f = func_map[i]; f.v == the.value()){
                        return f.name;
                    }
                }
                return "not func-key";
            };

            u08 & operator[](uxx index) {
                return values[index];
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::io_private_tty_key_t::tty_key_t;
    namespace func_key{
        using namespace ::mixc::io_private::func_key;
    }
}
