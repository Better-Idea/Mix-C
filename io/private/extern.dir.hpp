#ifndef xpack_io_private_dir
#define xpack_io_private_dir
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_dir
#include"io/dir.hpp"
#include"memop/copy.hpp"
#pragma pop_macro("xuser")

#if xis_windows
#include<Windows.h>

namespace mixc::io_dir::origin{
    void dir::remove() const{
        RemoveDirectoryA(path);
    }

    void dir::move_to(inc::c08 new_path) const{
        SHFILEOPSTRUCT opr{};
        opr.wFunc   = FO_MOVE;
        opr.pFrom   = path;
        opr.pTo     = new_path;
        opr.fFlags  = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
        SHFileOperationA(& opr);
    }

    void dir::copy_to(inc::c08 new_path) const{
        SHFILEOPSTRUCT opr{};
        opr.wFunc   = FO_COPY;
        opr.pFrom   = path;
        opr.pTo     = new_path;
        opr.fFlags  = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
        SHFileOperationA(& opr);
    }

    bool dir::is_exist() const {
        DWORD attribute = GetFileAttributesA(path);
        return INVALID_FILE_ATTRIBUTES != attribute and 0 != (attribute & FILE_ATTRIBUTE_DIRECTORY);
    }

    void dir::foreach_template(bool for_file, inc::c08 path, inc::can_callback< loop_t(asciis path) > invoke){
        char root[255]  = "";
        inc::copy(root, path, path.length());
        inc::copy(root + path.length(), "/*", 3);

        auto info       = WIN32_FIND_DATAA{};
        auto h          = FindFirstFileA(root, xref info);

        if (h == INVALID_HANDLE_VALUE){
            return;
        }

        FindNextFileA(h, xref info); // skip .
        FindNextFileA(h, xref info); // skip ..

        while(FindNextFileA(h, xref info)){
            auto is_dir = 0 != (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

            if (for_file ^ is_dir){
                if (invoke(info.cFileName) == loop_t::finish){
                    break;
                }
            }
        }

        CloseHandle(h);
    }
}

#else
#include<unistd.h>
#include<sys/dir.h>
#include<sys/stat.h>
namespace mixc::io_dir::origin{
    dir::dir() : fd(-1), path("") {}

    u64 dir::length() const {
        struct stat sta = {};
        stat(path, & sta);
        return sta.st_size;
    }

    dir & dir::open(asciis path, access_mode_t mode, bstate_t * result) const {
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

    dir & dir::close() const {
        if (atom_swap<ixx>(& fd, -1) >= 0){
            ::close(fd);
        }
        return thex;
    }

    dir & dir::forward(u64 offset) const {
        ::lseek(the.fd, 0 - offset, SEEK_CUR);
        return thex;
    }

    dir & dir::backward(u64 offset) const {
        ::lseek(the.fd, offset, SEEK_CUR);
        return thex;
    }

    dir & dir::seek(i64 offset) const {
        ::lseek(the.fd, offset, offset > 0 ? SEEK_SET : SEEK_END);
        return thex;
    }

    uxx dir::read(voidp buffer, uxx bytes) const {
        return ::read(the.fd, buffer, bytes);
    }

    uxx dir::write(void const * buffer, uxx bytes) const {
        return ::write(the.fd, buffer, bytes);
    }
}

#endif
#endif
