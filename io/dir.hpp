#ifndef xpack_io_dir
#define xpack_io_dir
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_dir::inc
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"interface/can_callback.hpp"
#include"macro/xexport.hpp"
#include"macro/xitr_foreach.hpp"
#include"macro/xref.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_dir::origin{
    xstruct(
        xname(dir),
        xpubb(inc::disable_copy),
        xprif(m_path,  inc::c08)
    )
        using final_t = the_t;
    public:
        struct subitem{
            subitem(inc::c08 path, bool for_file) : 
                path(path), for_file(for_file){}

            xitr_foreach(asciis)
        private:
            inc::c08 path;
            bool     for_file;

            template<auto mode_v, class invoke_t>
            void foreach_template(invoke_t invoke) const {
                uxx    index    = 0;
                loop_t state    = loop_t::go_on;

                dir::foreach_template(for_file, path, [&](asciis name){
                    state       = inc::itr_switch<mode_v>(xref(index), invoke, name);
                    return state;
                });
            }
        };

        dir();
        dir(inc::c08 path) : m_path(path){}

        bstate_t    remove()                                const;
        bstate_t    move_to(inc::c08 new_path)              const;
        bstate_t    copy_to(inc::c08 new_path)              const;
        bstate_t    create()                                const;
        bstate_t    as_cwd()                                const;
        bool        is_exist()                              const;

        auto subfiles() const {
            return subitem(m_path, true);
        }

        auto subdirs() const {
            return subitem(m_path, false);
        }

    private:
        static void foreach_template(bool for_file, inc::c08 path, inc::icallback< loop_t(asciis name) > invoke);
    $
}

#endif

xexport_space(mixc::io_dir::origin)
