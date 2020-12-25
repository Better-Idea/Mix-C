#ifndef xpack_io_dir
#define xpack_io_dir
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_dir::inc
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"interface/can_callback.hpp"
#include"macro/xitr_foreach.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_dir::origin{
    xstruct(
        xname(dir),
        xpubb(inc::disable_copy),
        xprif(path,  inc::c08)
    )
        using final = the_t;
    public:
        struct subitem{
            subitem(inc::c08 path, bool for_file) : 
                path(path), for_file(for_file){}

            xitr_foreach(asciis)
        private:
            inc::c08 path;
            bool     for_file;

            template<auto mode, class invoke_t>
            void foreach_template(invoke_t invoke) const {
                uxx    index = 0;
                loop_t state = loop_t::go_on;

                dir::foreach_template(for_file, path, [&](asciis name){
                    xitr_switch(mode, index, state, invoke, name);
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
        void    as_cwd()                                const;

        auto subfiles() const {
            return subitem(path, true);
        }

        auto subdirs() const {
            return subitem(path, false);
        }

    private:
        static void foreach_template(bool for_file, inc::c08 path, inc::icallback< loop_t(asciis name) > invoke);
    $
}

#endif

xexport_space(mixc::io_dir::origin)
