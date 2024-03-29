#ifndef xpack_io_tty
#define xpack_io_tty
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_tty::inc
#include"configure/switch.hpp"
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"io/private/tty.hpp"
#include"io/private/tty_color_t.hpp"
#include"io/private/tty_key.hpp"
#include"lang/cxx/ph.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#include"meta/has_cast.hpp"
#include"utils/memory.hpp"

#undef  xuser
#define xuser mixc::io_tty::ext
#include"lang/cxx.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_tty{
    xstruct(
        xname(tty_t)
    )
        using final_t = tty_t;

        xpubget_pubsetx(backcolor, inc::tty_color_t)
            xr { return inc::backcolor(); }
            xw { inc::backcolor(value); }

        xpubget_pubsetx(cursor_visiable, bool)
            xr { return inc::cursor_visiable(); }
            xw { inc::cursor_visiable(value); }

        xpubget_pubsetx(forecolor, inc::tty_color_t)
            xr { return inc::forecolor(); }
            xw { inc::forecolor(value); }

    public:
        void clear() const {
            inc::clear();
        }

        void flush() const {
            inc::print_flush();
        }

        void write(char chr){
            inc::print_core(& chr, 1);
        }

        template<class a0_t, class ... args_t>
        void write(a0_t const & first, args_t const & ... list) const {
            using namespace inc::ph;
            inc::c08  content;
            char      buf_stack[128];
            char *    buf_heap   = nullptr;
            auto &&   format     = [&](inc::c08 fmt, auto && ph){
                return ph.format(fmt, [&](uxx length) {
                    auto ptr         = buf_stack;

                    if (length >= sizeof(buf_stack) / sizeof(buf_stack[0])){
                        ptr = buf_heap = inc::memory::alloc<char>(
                            inc::memory::size(length + 1)
                        );
                    }
                    ptr[length] = '\0';
                    return ptr;
                });
            };

            if constexpr (inc::has_cast<inc::c08, a0_t const &> and sizeof...(list) > 0){
                content         = format(first, v{list...});
            }
            else{
                content         = format(inc::c08{}, v{first, list...});
            }

            if (inc::print_core(asciis(content), content.length()); buf_heap != nullptr){
                inc::memory::free(buf_heap, inc::memory::size(content.length() + 1));
            }
        }

        void write_line(){
            inc::print_core(xlocal_endl, xlocal_endl_length);
        }
        
        template<class a0_t, class ... args_t>
        void write_line(a0_t const & first, args_t const & ... list) const {
            write(first, list..., xlocal_endl);
        }

        inc::tty_key read_key() const {
            return inc::read_key(false);
        }

        ext::c08 read_line(inc::ialloc<char> alloc) const {
            return inc::read_line(alloc);
        }

        ext::c16 read_line(inc::ialloc<char16_t> alloc) const {
            return inc::read_line(alloc);
        }
    $
}

namespace mixc::io_tty::origin{
    using inc::tty_color_t;
    using inc::tty_key;

    inline tty_t tty;
}

#endif
#include"lang/cxx/ph.hpp"

xexport_space(mixc::io_tty::origin)