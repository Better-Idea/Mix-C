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
                f9,
                f10,
                f11,
                f12,
                enter,
                left,
                top,
                right,
                buttom,
                end,
                home,
                ins,
                del,
                page_up,
                page_down,
                backspace,
            };
        }

        enum key_type : u08{
            is_ascii   = 0x01,
            is_func    = 0x02,
            has_shift  = 0x04,
            has_alt    = 0x08,
            has_ctrl   = 0x10,
        };
    }

    namespace mixc::io_private_tty_key_t{
        using namespace mixc::io_private;
        using namespace mixc::io_private::func_key;

        struct pair{ u08 v; asciis name; };

        inline auto const & func_map = inc::array_view {
            pair { esc       , "escape"    },
            pair { f1        , "f1"        },
            pair { f2        , "f2"        },
            pair { f3        , "f3"        },
            pair { f4        , "f4"        },
            pair { f5        , "f5"        },
            pair { f6        , "f6"        },
            pair { f7        , "f7"        },
            pair { f8        , "f8"        },
            pair { f9        , "f9"        },
            pair { f10       , "f10"       },
            pair { f11       , "f11"       },
            pair { f12       , "f12"       },
            pair { enter     , "enter"     },
            pair { left      , "left"      },
            pair { top       , "top"       },
            pair { right     , "right"     },
            pair { buttom    , "buttom"    },
            pair { end       , "end"       },
            pair { home      , "home"      },
            pair { ins       , "insert"    },
            pair { del       , "delele"    },
            pair { page_up   , "page_up"   },
            pair { page_down , "page_down" },
            pair { backspace , "backspace" },
        };


        struct tty_key_t{
            static constexpr uxx max_length = 6;
            using items = u08[max_length];
            using final = tty_key_t;

            xgc_fields(
                xiam(tty_key_t),
                xpri(type,   key_type),
                xpri(values, items),
                xpri(count,  uxx)
            );
        public:
            constexpr tty_key_t() :
                type{}, values{}, count{}{}

            xpubget_pubset(is_ascii, bool){
                xr { return inc::bit(the.type, key_type::is_ascii); }
                xw { inc::bit(& the.type, key_type::is_ascii, value); }
            };

            xpubget_pubset(is_func, bool){
                xr { return inc::bit(the.type, key_type::is_func); }
                xw { inc::bit(& the.type, key_type::is_func, value); }
            };

            xpubget_pubset(has_shift, bool){
                xr { return inc::bit(the.type, key_type::has_shift); }
                xw { inc::bit(& the.type, key_type::has_shift, value); }
            };

            xpubget_pubset(has_alt, bool){
                xr { return inc::bit(the.type, key_type::has_alt); }
                xw { inc::bit(& the.type, key_type::has_alt, value); }
            };

            xpubget_pubset(has_ctrl, bool){
                xr { return inc::bit(the.type, key_type::has_ctrl); }
                xw { inc::bit(& the.type, key_type::has_ctrl, value); }
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

            xpubget(multi_ascii, asciis) {
                return (asciis)the.values;
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
