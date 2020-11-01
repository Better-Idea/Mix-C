#ifndef xpack_io_private_dir
#define xpack_io_private_dir
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_private_dir::inc
#include"io/dir.hpp"
#include"io/private/path_buffer.hpp"
#include"lang/cxx/strcat.hpp"
#include"lang/cxx.hpp"
#include"memop/copy.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xuser")

#if xis_windows
#include<Windows.h>
#pragma comment(lib, "Shell32.lib")

namespace mixc::io_dir::cpp{
    using namespace mixc::io_private_dir::inc;
}

namespace mixc::io_dir::origin{
    void dir::remove() const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(path);
        RemoveDirectoryA(source);
        buf.free(source);
    }

    void dir::move_to(inc::c08 new_path) const{
        auto && opr     = SHFILEOPSTRUCTA{};
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(path);
        auto    target  = buf.alloc(new_path);
        opr.wFunc       = FO_MOVE;
        opr.pFrom       = (asciis)source;
        opr.pTo         = (asciis)target;
        opr.fFlags      = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
        SHFileOperationA(& opr);
        buf.free(target);
        buf.free(source);
    }

    void dir::copy_to(inc::c08 new_path) const{
        auto && opr     = SHFILEOPSTRUCTA{};
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(path);
        auto    target  = buf.alloc(new_path);
        opr.wFunc       = FO_COPY;
        opr.pFrom       = (asciis)source;
        opr.pTo         = (asciis)target;
        opr.fFlags      = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
        SHFileOperationA(& opr);
        buf.free(target);
        buf.free(source);
    }

    bool dir::is_exist() const {
        auto && opr     = SHFILEOPSTRUCTA{};
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(path);
        auto    attr    = GetFileAttributesA((asciis)source);
        buf.free(source);
        return INVALID_FILE_ATTRIBUTES != attr and 0 != (attr & FILE_ATTRIBUTE_DIRECTORY);
    }

    void dir::foreach_template(bool for_file, inc::c08 path, inc::can_callback< loop_t(asciis path) > invoke){
        auto info       = WIN32_FIND_DATAA{};
        auto h          = ([](auto path, auto * info){
            char root[256]  = "";
            cpp::copy(root, path, path.length());
            cpp::copy(root + path.length(), "\\*", 3);

            auto h          = FindFirstFileA(root, info);
            return h;
        })(path, xref info);

        if (h == INVALID_HANDLE_VALUE){
            return;
        }

        FindNextFileA(h, xref info); // skip "../"

        while(FindNextFileA(h, xref info)){
            auto is_dir = 0 != (info.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);

            if (for_file ^ is_dir){
                if (invoke(info.cFileName) == loop_t::finish){
                    break;
                }
            }
        }
    }

    void dir::create() const {
        char buf[256];
        auto mkdir = cpp::c08{"mkdir \""}.strcat({path, "\""}, [&](uxx length){
            auto ptr = buf;

            if (length >= sizeof(buf) - 1){
                ptr = cpp::alloc<char>(cpp::memory_size{length + 1});
            }

            ptr[length] = '\0';
            return ptr;
        });

        system((asciis)mkdir);

        if (mkdir.operator char *() != buf){
            cpp::free<char>(mkdir, cpp::memory_size{mkdir.length() + 1});
        }
    }
}

#else

namespace mixc::io_dir::origin{
    
}

#endif
#endif
