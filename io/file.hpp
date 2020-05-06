#ifndef xpack_io_file
#define xpack_io_file
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::io_file
        #include<unistd.h>
        #include<sys/file.h>
        #include"define/base_type.hpp"
        #include"dumb/disable_copy.hpp"
        #include"interface/seqptr.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xindex_rollback.hpp"
        #include"lock/atom_swap.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::io_file{
        enum class access_mode_t{
            read_only   = 0x1,
            write_only  = 0x2,
            read_write  = 0x3,
        };

        namespace access_mode{
            constexpr auto read_only  = access_mode_t::read_only;
            constexpr auto write_only = access_mode_t::write_only;
            constexpr auto read_write = access_mode_t::read_write;
        }

        struct file : inc::disable_copy {
            xgc_fields(
                xiam(file),
                xpri(fd,   mutable i32),
                xpri(path, mutable asciis)
            );
            using final = the_t;
        public:
            file() : fd(-1), path("") {}
            file(asciis path, access_mode_t mode, bstate * result = nullptr) {
                the.open(path, mode, result);
            }
            ~file(){
                the.close();
            }

            u64 length() const {
                if (struct stat s; ::stat(the.path, & s) == 0){
                    return s.st_size;
                }
                return 0;
            }

            final & open(asciis path, access_mode_t mode, bstate * result = nullptr) const {
                i32 type = 0;

                switch(mode){
                case access_mode_t::read_only:  type = O_RDONLY; break;
                case access_mode_t::write_only: type = O_WRONLY; break;
                case access_mode_t::read_write: type = O_RDWR;   break;
                }

                the.close();
                the.path = path;
                the.fd   = ::open(path, type);

                if (result != nullptr){
                    result[0] = the.fd == -1 ? fail : success;
                }
                return thex;
            }

            final & close() const {
                if (inc::atom_swap(& fd, 0) > 0){
                    ::close(fd);
                }
                return thex;
            }

            final & forward(u64 offset) const {
                ::lseek(the.fd, 0 - offset, SEEK_CUR);
                return thex;
            }

            final & backward(u64 offset) const {
                ::lseek(the.fd, offset, SEEK_CUR);
                return thex;
            }

            final & seek(i64 offset) const {
                ::lseek(the.fd, offset, offset > 0 ? SEEK_SET : SEEK_END);
                return thex;
            }

            template<class item_t>
            uxx read(inc::seqptr<item_t> seq) const {
                return ::read(the.fd, seq, seq.length() * sizeof(item_t));
            }

            template<class item_t>
            uxx write(inc::seqptr<item_t> seq) const {
                return ::write(the.fd, seq, seq.length() * sizeof(item_t));
            }

        };
    }
#endif

namespace xuser::inc{
    using ::mixc::io_file::file;
}

namespace xuser::inc::access_mode{
    using namespace ::mixc::io_file::access_mode;
}
