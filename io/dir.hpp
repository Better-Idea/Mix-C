#ifndef xpack_io_dir
#define xpack_io_dir
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_dir::inc
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"interface/can_callback.hpp"
#include"interface/iterator.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_dir::origin{
    xstruct(
        xname(dir),
        xpubb(inc::disable_copy),
        xprif(path, mutable inc::c08)
    )
        using final = the_t;
    public:
        struct subitem{
            subitem(inc::c08 path, bool for_file) : 
                path(path), for_file(for_file){}

            xitr_foreach(asciis)
            xitr_foreach_const(asciis)
        private:
            inc::c08 path;
            bool     for_file;

            template<auto mode, class invoke_t>
            void foreach_template(invoke_t invoke) const {
                uxx    index = 0;
                loop_t state = loop_t::go_on;

                dir::foreach_template(for_file, path, [&](asciis path){
                    xitr_switch(mode, index, state, invoke, path);
                    return state;
                });
            }
        };

        dir();
        dir(inc::c08 path) : path(path){}

        void    remove()                                const;
        void    move_to(inc::c08 new_path)              const;
        void    copy_to(inc::c08 new_path)              const;
        bool    is_exist()                              const;
        void    create()                                const;

        auto subfiles() const {
            return subitem(path, true);
        }

        auto subdirs() const {
            return subitem(path, false);
        }
    private:
        static void foreach_template(bool for_file, inc::c08 path, inc::can_callback< loop_t(asciis path) > invoke);
    $
}

#endif

xexport_space(mixc::io_dir::origin)
