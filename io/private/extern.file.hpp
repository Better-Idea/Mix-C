#ifndef xpack_io_private_file
#define xpack_io_private_file
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_private_file::inc
#include"configure.hpp"
#include"io/file.hpp"
#include"io/dir.hpp"
#include"io/private/path_buffer.hpp"
#include"lang/cxx/index_of_last.hpp"
#include"lang/cxx.hpp"
#include"lock/atom_swap.hpp"
#include"macro/xindex_rollback.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_file::cpp{
    using namespace ::mixc::io_private_file::inc;
}

namespace mixc::io_file::origin{
    file::file(file && value){
        the.path    = value.path;
        the.fd      = cpp::atom_swap<ixx>(& value.fd, -1);
    }
}

#if xis_windows
#include<Windows.h>
namespace mixc::io_file::origin{
    file::file() : fd(0), path("") {}

    u64 file::length() const {
        u64 size = 0;
        GetFileSizeEx(HANDLE(fd), PLARGE_INTEGER(& size));
        return size;
    }

    file & file::open(inc::c08 path, access_mode_t mode, bstate_t * result) const {
        auto type = 0;
        auto i    = cpp::c08{path}.index_of_last({'/','\\'});

        if (i != not_exist){
            if (auto folder = cpp::c08{path}.length(i); cpp::dir{folder}.is_exist() == false){
                cpp::dir{folder}.create();
            }
        }

        switch(mode){
        case access_mode_t::read_only:  type = GENERIC_READ;                    break;
        case access_mode_t::write_only: type = GENERIC_WRITE;                   break;
        case access_mode_t::read_write: type = GENERIC_READ | GENERIC_WRITE;    break;
        }

        the.close();
        the.path = path;
        the.fd   = (ixx)CreateFileA(path, type, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_NEW, 0, NULL);

        if (the.fd == -1){
            the.fd = (ixx)CreateFileA(path, type, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
        }
        if (result != nullptr){
            result[0] = the.fd == -1 ? bstate_t::fail : bstate_t::success;
        }
        return thex;
    }

    file & file::close() const {
        if (cpp::atom_swap<ixx>(& fd, 0) > 0){
            CloseHandle(HANDLE(fd));
        }
        return thex;
    }

    file & file::forward(u64 offset) const {
        return backward(u64(0) - offset);
    }

    file & file::backward(u64 offset) const {
        SetFilePointerEx(HANDLE(the.fd), *PLARGE_INTEGER(& offset), NULL, FILE_CURRENT);
        return thex;
    }

    file & file::seek(i64 offset) const {
        if (offset < 0){
            offset = -offset - 1;
            SetFilePointerEx(HANDLE(the.fd), *PLARGE_INTEGER(& offset), NULL, FILE_END);
        }
        else{
            SetFilePointerEx(HANDLE(the.fd), *PLARGE_INTEGER(& offset), NULL, FILE_BEGIN);
        }
        return thex;
    }

    uxx file::read(voidp buffer, uxx bytes) const {
        u64 size = 0;
        ReadFile(HANDLE(the.fd), buffer, DWORD(bytes), LPDWORD(& size), NULL);
        return uxx(size);
    }

    uxx file::write(void const * buffer, uxx bytes) const {
        u64 size = 0;
        WriteFile(HANDLE(the.fd), buffer, DWORD(bytes), LPDWORD(& size), NULL);
        return uxx(size);
    }

    void file::remove() const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);
        DeleteFileA(source);
        buf.free(source);
    }

    void file::move_to(inc::c08 new_path) const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);
        auto    target  = buf.alloc(new_path);
        MoveFileA(source, target);
        buf.free(target);
        buf.free(source);
    }

    void file::copy_to(inc::c08 new_path) const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);
        auto    target  = buf.alloc(new_path);
        CopyFileA(source, target, true/*override*/);
        buf.free(target);
        buf.free(source);
    }

    bool file::is_exist() const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);
        auto word = GetFileAttributesA(the.path);
        buf.free(source);
        return word != INVALID_FILE_ATTRIBUTES and 0 == (word & FILE_ATTRIBUTE_DIRECTORY);
    }
}

#else
#include<unistd.h>
#include<sys/file.h>
#include<sys/stat.h>
namespace mixc::io_file::origin{
    file::file() : fd(-1), path("") {}

    u64 file::length() const {
        struct stat sta = {};
        stat(path, & sta);
        return sta.st_size;
    }

    file & file::open(inc::c08 path, access_mode_t mode, bstate_t * result) const {
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
            result[0] = the.fd == -1 ? bstate_t::fail : bstate_t::success;
        }
        return thex;
    }

    file & file::close() const {
        if (cpp::atom_swap<ixx>(& fd, -1) >= 0){
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

    uxx file::write(void const * buffer, uxx bytes) const {
        return ::write(the.fd, buffer, bytes);
    }
}
#endif
#endif
