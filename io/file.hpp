#ifndef xpack_io_file
#define xpack_io_file
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_file::inc
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"dumb/move.hpp"
#include"interface/seqptr.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#include"macro/xnew.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_file::origin{
    enum class access_mode_t{
        read_only   = 0x1,
        write_only  = 0x2,
        read_write  = 0x3,
    };

    xstruct(
        xname(file),
        xpubb(inc::disable_copy),
        xprif(fd,    ixx),
        xprif(path,  inc::c08)
    )
        using final = the_t;
    public:
        file();
        file(file &&);
        file(inc::c08 path) : fd(-1), path(path) {}
        file(inc::c08 path, access_mode_t mode, bstate_t * result = nullptr) {
            the.open(path, mode, result);
        }
        ~file(){
            the.close();
        }

        final & operator=(file && value){
            if (this == & value){
                return thex;
            }
            the.close();
            xnew(this) file(inc::move(value));
            return thex;
        }

        u64     length()                                const;
        final & open(inc::c08 path, access_mode_t mode, bstate_t * result = nullptr) const;
        final & close()                                 const;
        final & forward(u64 offset)                     const;
        final & backward(u64 offset)                    const;
        final & seek(i64 offset)                        const;
        uxx     read(voidp buffer, uxx bytes)           const;
        uxx     write(void const * buffer, uxx bytes)   const;

        void    remove()                                const;
        void    move_to(inc::c08 new_path)              const;
        void    copy_to(inc::c08 new_path)              const;
        bool    is_exist()                              const;

        template<class item_t>
        uxx read(inc::seqptr<item_t> seq) const {
            return read(seq, seq.length() * sizeof(item_t));
        }

        template<class item_t>
        uxx write(inc::seqptr<item_t> seq) const {
            return write(seq, seq.length() * sizeof(item_t));
        }
    $
}

#endif

xexport_space(mixc::io_file::origin)
