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
#pragma comment(lib, "Shell32.lib")

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

namespace mixc::io_dir::origin{
    
}

#endif
#endif
