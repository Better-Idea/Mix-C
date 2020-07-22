#ifndef xpack_io_tty
#define xpack_io_tty
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_tty
#include"io/private/tty.hpp"
#include"io/private/tty_color_t.hpp"
#include"io/private/tty_key.hpp"
#include"lang/cxx/ph.hpp"
#include"lang/cxx.hpp"
#include"macro/xstruct.hpp"
#include"mixc.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_tty::origin{
    using inc::tty_color_t;
    using inc::tty_key;

    xstruct(
        xname(tty_t)
    )
        using final = tty_t;

        tty_t(){
            // backcolor(inc::tty_color::black);
            forecolor(inc::tty_color::light_gray);
            cursor_visiable(true);
        }

        xpubget_pubsetx(backcolor, inc::tty_color_t)
            xr { return inc::backcolor(); }
            xw { inc::backcolor(value); }

        xpubget_pubsetx(cursor_visiable, bool)
            xr { return inc::cursor_visiable(); }
            xw { inc::cursor_visiable(value); }

        xpubget_pubsetx(forecolor, inc::tty_color_t)
            xr { return inc::forecolor(); }
            xw { inc::forecolor(value); }

        xpubgetx(read_key, inc::tty_key){
            return inc::read_key(false);
        }

    public:
        final & clear() const {
            inc::clear();
            return thex;
        }

        final & flush() const {
            inc::print_flush();
            return thex;
        }
        
        template<class a0, class ... args>
        final & write(a0 const & first, args const & ... list) const {
            using namespace inc::ph;
            char   buf_stack[128];
            char * buf_heap   = nullptr;
            auto   buf_length = 0;
            auto   content    = phg { first, list... } >> [&](uxx length) {
                auto ptr = buf_stack;

                if (length >= sizeof(buf_stack) / sizeof(buf_stack[0])){
                    ptr = buf_heap = inc::alloc<char>(
                        inc::memory_size(length + 1) // 1 for '\0'
                    );
                }

                buf_length  = length;
                ptr[length] = '\0';
                return ptr;
            };

            if (inc::print_core(asciis(content), content.length()); buf_heap != nullptr){
                inc::free(buf_heap, inc::memory_size(buf_length + 1));
            }
            return thex;
        }
        
        template<class ... args>
        final & write_line(args const & ... list) const {
            write(list..., '\n');
            return thex;
        }
    $

    inline static tty_t tty;
}

#endif

namespace xuser::inc{
    using namespace ::mixc::io_tty::origin;
}

namespace xuser::inc::ph{
    using namespace ::mixc::lang_cxx_ph::ph;
}
