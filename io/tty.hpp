#ifndef xpack_io_tty
#define xpack_io_tty
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_tty::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"io/private/tty.hpp"
#include"io/private/tty_color_t.hpp"
#include"io/private/tty_key.hpp"
#include"lang/cxx/ph.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_tty{
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
        void clear() const {
            inc::clear();
        }

        void flush() const {
            inc::print_flush();
        }

        template<class a0, class ... args>
        void write(a0 const & first, args const & ... list) const {
            using namespace inc::ph;
            char      buf_stack[128];
            char *    buf_heap   = nullptr;
            inc::c08  content    = phg { first, list... } >> [&](uxx length) {
                auto ptr         = buf_stack;

                if (length > sizeof(buf_stack) / sizeof(buf_stack[0])){
                    ptr = buf_heap = inc::alloc<char>(
                        inc::memory_size(length)
                    );
                }
                return ptr;
            };

            if (inc::print_core(asciis(content), content.length()); buf_heap != nullptr){
                inc::free(buf_heap, inc::memory_size(content.length()));
            }
        }
        
        template<class ... args>
        void write_line(args const & ... list) const {
            write(list..., '\n');
        }

        void read_line(inc::can_alloc<char> alloc) const {
            inc::read_line(alloc);
        }

        void read_line(inc::can_alloc<char16_t> alloc) const {
            inc::read_line(alloc);
        }
    $

}

namespace mixc::io_tty::origin{
    using inc::tty_color_t;
    using inc::tty_key;

    static inline tty_t tty;
}

#endif
#include"lang/cxx/ph.hpp"

xexport_space(mixc::io_tty::origin)