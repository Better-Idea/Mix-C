#ifndef xpack_io_tty
#define xpack_io_tty
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_tty::inc
#include"configure.hpp"
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"io/private/tty.hpp"
#include"io/private/tty_color_t.hpp"
#include"io/private/tty_key.hpp"
#include"lang/cxx/ph.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#include"memory/allocator.hpp"
#include"meta/has_cast.hpp"
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

        void write(char chr){
            inc::print_core(& chr, 1);
        }

        template<class a0, class ... args>
        void write(a0 const & first, args const & ... list) const {
            using namespace inc::ph;
            inc::c08  content;
            char      buf_stack[128];
            char *    buf_heap   = nullptr;
            auto &&   format     = [&](inc::c08 fmt, auto && ph){
                return ph.format(fmt, [&](uxx length) {
                    auto ptr         = buf_stack;

                    if (length > sizeof(buf_stack) / sizeof(buf_stack[0])){
                        ptr = buf_heap = inc::alloc<char>(
                            inc::memory_size(length)
                        );
                    }
                    return ptr;
                });
            };

            if constexpr (inc::has_cast<inc::c08, a0 const &> and sizeof...(list) > 0){
                content         = format(first, v{list...});
            }
            else{
                content         = format(inc::c08{}, v{first, list...});
            }

            if (inc::print_core(asciis(content), content.length()); buf_heap != nullptr){
                inc::free(buf_heap, inc::memory_size(content.length()));
            }
        }

        void write_line(){
            inc::print_core(xendline, 1);
        }
        
        template<class a0, class ... args>
        void write_line(a0 const & first, args const & ... list) const {
            write(first, list..., xendline);
        }

        void read_line(inc::ialloc<char> alloc) const {
            inc::read_line(alloc);
        }

        void read_line(inc::ialloc<char16_t> alloc) const {
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