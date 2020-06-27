#ifndef xpack_io_file
#define xpack_io_file
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::io_file
    #include"mixc.hpp"
    #include"dumb/disable_copy.hpp"
    #include"interface/seqptr.hpp"

    namespace mixc::io_file::origin{
        enum class access_mode_t{
            read_only   = 0x1,
            write_only  = 0x2,
            read_write  = 0x3,
        };

        struct file : inc::disable_copy {
            xgc_fields(
                xiam(file),
                xpri(fd,   mutable i32),
                xpri(path, mutable asciis)
            );
            using final = the_t;
        private:
            file(i32 stdfd) : fd(stdfd), path(""){}
        public:
            file() : fd(-1), path("") {}
            file(asciis path, access_mode_t mode, bstate * result = nullptr) {
                the.open(path, mode, result);
            }
            ~file(){
                the.close();
            }

            u64     length() const;
            final & open(asciis path, access_mode_t mode, bstate * result = nullptr) const;
            final & close() const;
            final & forward(u64 offset) const;
            final & backward(u64 offset) const;
            final & seek(i64 offset) const;
            uxx     read(voidp buffer, uxx bytes) const;
            uxx     write(void const * buffer, uxx bytes) const;

            template<class item_t>
            uxx read(inc::seqptr<item_t> seq) const {
                return read(seq, seq.length() * sizeof(item_t));
            }

            template<class type>
            uxx read(type * object){
                return read(object, sizeof(type));
            }

            template<class item_t>
            uxx write(inc::seqptr<item_t> seq) const {
                return write(seq, seq.length() * sizeof(item_t));
            }

            template<class type>
            uxx write(type const & object){
                return read(xref object, sizeof(type));
            }
        };
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using namespace ::mixc::io_file::origin;
}
