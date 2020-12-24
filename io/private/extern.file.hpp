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
#else
#include<stdio.h>
#include<sys/file.h>
#include<sys/stat.h>
#include<unistd.h>
#endif

namespace mixc::io_file::origin{
    file::file() : fd(-1), path("") {}

    u64 file::length() const {
        #if xis_windows

        u64 size = 0;
        ::GetFileSizeEx(HANDLE(fd), PLARGE_INTEGER(& size));
        return size;

        #elif xis_linux

        struct stat sta = {};
        ::stat(path, & sta);
        return sta.st_size;

        #else
        #error "os miss match"
        #endif
    }

    file & file::open(inc::c08 path, access_mode_t mode, bstate_t * result) const {
        auto type = 0;
        auto i    = cpp::c08{path}.index_of_last({'/','\\'});

        if (i != not_exist and mode != access_mode_t::read_only){
            if (auto folder = cpp::c08{path}.length(i); cpp::dir{folder}.is_exist() == false){
                cpp::dir{folder}.create();
            }
        }

        the.close();
        the.path = path;

        #if xis_windows

        switch(mode){
        case access_mode_t::read_only:  type = GENERIC_READ;                    break;
        case access_mode_t::write_only: type = GENERIC_WRITE;                   break;
        case access_mode_t::read_write: type = GENERIC_READ | GENERIC_WRITE;    break;
        }

        the.fd      = (ixx)::CreateFileA(path, type, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, CREATE_NEW, 0, NULL);

        if (the.fd == -1){
            the.fd  = (ixx)::CreateFileA(path, type, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
        }

        #elif xis_linux

        switch(mode){
        case access_mode_t::read_only:  type = O_RDONLY; break;
        case access_mode_t::write_only: type = O_WRONLY; break;
        case access_mode_t::read_write: type = O_RDWR;   break;
        }

        the.fd      = ::open(path, type);

        #else
        #error "os miss match"
        #endif

        if (result != nullptr){
            result[0] = the.fd <= 0 ? bstate_t::fail : bstate_t::success;
        }
        return thex;
    }

    file & file::close() const {
        if (cpp::atom_swap<ixx>(& fd, 0) <= 0){
            #if xis_windows
            ::CloseHandle(HANDLE(fd));
            #elif xis_linux
            ::close(fd);
            #else
            #error "os miss match"
            #endif
        }
        return thex;
    }

    file & file::forward(u64 offset) const {
        return backward(u64(0) - offset);
    }

    file & file::backward(u64 offset) const {
        #if xis_windows
        ::SetFilePointerEx(HANDLE(the.fd), *PLARGE_INTEGER(& offset), NULL, FILE_CURRENT);
        #elif xis_linux
        ::lseek(the.fd, offset, SEEK_CUR);
        #else
        #error "os miss match"
        #endif
        return thex;
    }

    file & file::seek(i64 offset) const {
        #if xis_windows

        if (offset < 0){
            offset = -offset - 1;
            ::SetFilePointerEx(HANDLE(the.fd), *PLARGE_INTEGER(& offset), NULL, FILE_END);
        }
        else{
            ::SetFilePointerEx(HANDLE(the.fd), *PLARGE_INTEGER(& offset), NULL, FILE_BEGIN);
        }

        #elif xis_linux
        ::lseek(the.fd, offset, offset > 0 ? SEEK_SET : SEEK_END);
        #else
        #error "os miss match"
        #endif
        return thex;
    }

    uxx file::read(voidp buffer, uxx bytes) const {
        if (the.fd <= 0){
            the.open(the.path, access_mode_t::read_only);
        }

        u64 size = 0;

        #if xis_windows
        ::ReadFile(HANDLE(the.fd), buffer, DWORD(bytes), LPDWORD(& size), NULL);
        #elif xis_linux
        size = (uxx)::read(the.fd, buffer, bytes);
        #else
        #error "os miss match"
        #endif

        if (size == not_exist){
            return 0;
        }
        return size;
    }

    uxx file::write(void const * buffer, uxx bytes) const {
        if (the.fd <= 0){
            the.open(the.path, access_mode_t::write_only);
        }

        u64 size = 0;

        #if xis_windows
        ::WriteFile(HANDLE(the.fd), buffer, DWORD(bytes), LPDWORD(& size), NULL);
        #elif xis_linux
        size = (uxx)::write(the.fd, buffer, bytes);
        #else
        #error "os miss match"
        #endif

        if (size == not_exist){
            return 0;
        }
        return size;
    }

    void file::remove() const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);

        #if xis_windows
        ::DeleteFileA(source);
        #elif xis_linux
        ::remove(source);
        #else
        #error "os miss match"
        #endif

        buf.free(source, the.path);
    }

    void file::move_to(inc::c08 new_path) const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);
        auto    target  = buf.alloc(new_path);
        #if xis_windows
        ::MoveFileA(source, target);
        #elif xis_linux
        ::rename(source, target);
        #else
        #error "os miss match"
        #endif
        buf.free(target, new_path);
        buf.free(source, the.path);
    }

    void file::copy_to(inc::c08 new_path) const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = (asciis)buf.alloc(the.path);
        auto    target  = (asciis)buf.alloc(new_path);

        #if xis_windows
        ::CopyFileA(source, target, true/*override*/);
        #elif xis_linux

        if (vfork() == 0){ // this thread will wait child finished
            execl("/bin/cp", "cp", source, target, NULL);
            _exit(-1);
        }

        #else
        #error "os miss match"
        #endif

        buf.free(target, new_path);
        buf.free(source, the.path);
    }

    bool file::is_exist() const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);

        #if xis_windows

        auto    word    = GetFileAttributesA(the.path);
        return  word != INVALID_FILE_ATTRIBUTES and 0 == (word & FILE_ATTRIBUTE_DIRECTORY);

        #elif xis_linux

        typedef struct stat meta_t;
        auto && meta    = meta_t{};
        auto    exist   = stat(source, & meta) != -1 and (meta.st_mode & S_IFDIR) != 0;
        return  exist;

        #else
        #error "os miss match"
        #endif

        buf.free(source, the.path);
    }
}

#endif
