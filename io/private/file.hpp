#ifndef xpack_io_private_file
#define xpack_io_private_file
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::io_private_file
    #include<unistd.h>
    #include<sys/file.h>
    #include<sys/stat.h>
    #include"define/base_type.hpp"
    #include"io/file.hpp"
    #include"lock/atom_swap.hpp"
    #include"macro/xindex_rollback.hpp"

    namespace mixc::io_file{
        using namespace xuser::inc;
    }

    namespace mixc::io_file::origin{
        u64 file::length() const {
            struct stat sta = {};
            stat(path, & sta);
            return sta.st_size;
        }

        file & file::open(asciis path, access_mode_t mode, bstate * result) const {
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

        file & file::close() const {
            if (atom_swap(& fd, 0) > 0){
                ::close(fd);
            }
            return thex;
        }

        file & file::forward(u64 offset) const {
            ::lseek(the.fd, 0 - offset, SEEK_CUR);
            return thex;
        }

        file & file::backward(u64 offset) const {
            ::lseek(the.fd, offset, SEEK_CUR);
            return thex;
        }

        file & file::seek(i64 offset) const {
            ::lseek(the.fd, offset, offset > 0 ? SEEK_SET : SEEK_END);
            return thex;
        }

        uxx file::read(voidp buffer, uxx bytes) const {
            return ::read(the.fd, buffer, bytes);
        }

        uxx file::write(voidp buffer, uxx bytes) const {
            return ::write(the.fd, buffer, bytes);
        }
    }
    #pragma pop_macro("xuser")
#endif
