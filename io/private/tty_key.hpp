#ifndef xpack_io_private_tty_key
#define xpack_io_private_tty_key
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::io_private_tty_key
    #include"algo/binary_search.hpp"
    #include"define/base_type.hpp"
    #include"docker/array.hpp"
    #include"macro/xstruct.hpp"
    #include"memop/bit.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::io_private_tty_key{
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
            is_char    = 0x01,
            is_func    = 0x02,
            has_shift  = 0x04,
            has_alt    = 0x08,
            has_ctrl   = 0x10,
        };

        struct pair{ u08 v; asciis name; };

        using namespace func_key;

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
    }

    namespace mixc::io_private_tty_key::origin{
        template<uxx size>
        using items_t = char[size];

        xstruct(
            xiam(tty_key),
            xitm(type, key_type),
            xitm(w08, items_t<5>),
            xitm(w16, char16_t)
        )
            using final = tty_key;
        public:
            constexpr tty_key() :
                type{}, w08{}, w16{}{}

            xpubget_pubsetx(is_char, bool)
                xr { return inc::bit(the.type, key_type::is_char); }
                xw { inc::bit(& the.type, key_type::is_char, value); }

            xpubget_pubsetx(is_func, bool)
                xr { return inc::bit(the.type, key_type::is_func); }
                xw { inc::bit(& the.type, key_type::is_func, value); }

            xpubget_pubsetx(has_shift, bool)
                xr { return inc::bit(the.type, key_type::has_shift); }
                xw { inc::bit(& the.type, key_type::has_shift, value); }

            xpubget_pubsetx(has_alt, bool)
                xr { return inc::bit(the.type, key_type::has_alt); }
                xw { inc::bit(& the.type, key_type::has_alt, value); }

            xpubget_pubsetx(has_ctrl, bool)
                xr { return inc::bit(the.type, key_type::has_ctrl); }
                xw { inc::bit(& the.type, key_type::has_ctrl, value); }

            xpubget_pubsetx(value, char16_t)
                xr { return the.w16; }
                xw { the.w16 = value; }

            xpubgetx(multi_bytes_char, asciis){
                return w08;
            }

            // 临时设施 ============================================
            xpubgetx(func_name, asciis){
                for(uxx i = 0; i < func_map.length(); i++){
                    if (auto & f = func_map[i]; f.v == the.value()){
                        return f.name;
                    }
                }
                return "not func-key";
            }
        $
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::io_private_tty_key::origin;
}

namespace xuser::inc::func_key{
    using namespace ::mixc::io_private_tty_key::func_key;
}
