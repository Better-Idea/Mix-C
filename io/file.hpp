#ifndef xpack_io_file
#define xpack_io_file
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::io_file
    #include"mixc.hpp"
    #include"dumb/disable_copy.hpp"
    #include"interface/seqptr.hpp"
    #include"macro/xstruct.hpp"
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
            xprif(fd,   mutable ixx),
            xprif(path, mutable asciis)
        )
            using final = the_t;
        public:
            file();
            file(asciis path) : path(path){}
            file(asciis path, access_mode_t mode, bstate * result = nullptr) {
                the.open(path, mode, result);
            }
            ~file(){
                the.close();
            }

            u64     length()                                const;
            final & open(asciis path, access_mode_t mode, bstate * result = nullptr) const;
            final & close()                                 const;
            final & forward(u64 offset)                     const;
            final & backward(u64 offset)                    const;
            final & seek(i64 offset)                        const;
            uxx     read(voidp buffer, uxx bytes)           const;
            uxx     write(void const * buffer, uxx bytes)   const;

            void    remove()                                const;
            void    move_to(asciis new_path)                const;
            void    copy_to(asciis new_path)                const;
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

namespace xuser::inc{
    using namespace ::mixc::io_file::origin;
}
