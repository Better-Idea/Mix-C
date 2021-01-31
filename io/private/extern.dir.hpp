#ifndef xpack_io_private_dir
#define xpack_io_private_dir
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_private_dir::inc
#include"configure.hpp"
#include"io/dir.hpp"
#include"io/private/path_buffer.hpp"
#include"lang/cxx/compare_fastly.hpp"
#include"lang/cxx/strcat.hpp"
#include"lang/cxx.hpp"
#include"macro/xdefer.hpp"
#include"memop/copy.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_dir::cpp{
    using namespace mixc::io_private_dir::inc;
}

#if xis_windows
#include<Windows.h>
#pragma comment(lib, "Shell32.lib")
#elif xis_linux
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#endif

namespace mixc::io_dir::origin{
    #if xis_linux

    template<class ... args>
    void dir_operator(asciis exe_path, asciis exe, args ... list){
        if (vfork() == 0){ // this thread will wait child finished
            execl(exe_path, exe, (asciis)list..., NULL);
            _exit(-1);
        }
    }
    #endif

    void dir::remove() const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);
        #if xis_windows
        ::RemoveDirectoryA(source);
        #elif xis_linux
        dir_operator("/bin/rm", "rm", "-rf", source, NULL);
        #else
        #error "os miss match"
        #endif
        buf.free(source, path);
    }

    void dir::move_to(inc::c08 new_path) const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);
        auto    target  = buf.alloc(new_path);

        #if xis_windows

        auto && opr     = SHFILEOPSTRUCTA{};
        opr.wFunc       = FO_MOVE;
        opr.pFrom       = (asciis)source;
        opr.pTo         = (asciis)target;
        opr.fFlags      = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
        SHFileOperationA(& opr);

        #elif xis_linux
        dir_operator("/bin/mv", "mv", "-r", source, target);
        #else
        #error "os miss match"
        #endif

        buf.free(target, new_path);
        buf.free(source, the.path);
    }

    void dir::copy_to(inc::c08 new_path) const{
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);
        auto    target  = buf.alloc(new_path);

        #if xis_windows

        auto && opr     = SHFILEOPSTRUCTA{};
        opr.wFunc       = FO_COPY;
        opr.pFrom       = (asciis)source;
        opr.pTo         = (asciis)target;
        opr.fFlags      = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
        SHFileOperationA(& opr);

        #elif xis_linux
        dir_operator("/bin/cp", "cp", "-r", source, target);
        #else
        #error "os miss match"
        #endif

        buf.free(target, new_path);
        buf.free(source, the.path);
    }

    bool dir::is_exist() const {
        auto && buf     = cpp::path_buffer{};
        auto    source  = buf.alloc(the.path);

        #if xis_windows

        auto && opr     = SHFILEOPSTRUCTA{};
        auto    attr    = GetFileAttributesA((asciis)source);
        auto    exist   = INVALID_FILE_ATTRIBUTES != attr and 0 != (attr & FILE_ATTRIBUTE_DIRECTORY);

        #elif xis_linux

        typedef struct stat meta_t;
        auto && meta    = meta_t{};
        auto    exist   = stat(source, & meta) != -1 and (meta.st_mode & S_IFDIR) != 0;

        #else
        #error "os miss match"
        #endif

        buf.free(source, path);
        return  exist;
    }

    void dir::foreach_template(bool for_file, inc::c08 path, inc::icallback< loop_t(asciis path) > invoke){
        auto go_on          = true;
        auto is_dir         = false;

        #if xis_windows

        auto entry          = WIN32_FIND_DATAA{};
        auto fd             = ([](auto path, auto * entry){
            char root[256];
            cpp::copy(root, path, path.length());
            cpp::copy(root + path.length(), "\\*", 3);
            auto fd         = FindFirstFileA(root, entry);
            return fd;
        })(path, xref entry);

        if (fd == INVALID_HANDLE_VALUE){
            return;
        }

        xdefer{
            ::CloseHandle(fd);
        };

        #define xentry_name     (entry.cFileName)
        #define xis_dir         (0 != (entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        #define xhas_next       (FindNextFileA(fd, xref entry) != 0)

        #elif xis_linux

        struct
        dirent  * entry;
        DIR     * fd;
        auto &&   buf       = cpp::path_buffer{};
        auto      source    = buf.alloc(path);

        if (fd = opendir(source); fd == nullptr){
            return;
        }
        else{
            entry           = readdir(fd);
        }

        xdefer{
            ::closedir(fd);
        };

        #define xentry_name     (entry->d_name)
        #define xis_dir         ((entry->d_type & DT_DIR) != 0)
        #define xhas_next       ((entry = readdir(fd)) != nullptr)

        #endif

        for(; go_on; go_on = xhas_next){
            if (is_dir = xis_dir; for_file ^ is_dir){
                if (cpp::c08{xentry_name}.compare_fastly(".") == 0 or
                    cpp::c08{xentry_name}.compare_fastly("..") == 0){
                    continue;
                }
                if (invoke(xentry_name) == loop_t::finish){
                    break;
                }
            }
        }

        #undef  xentry_name
        #undef  xis_dir
        #undef  xhas_next
    }

    void dir::create() const {
        #if xis_windows

        char buf[256];
        auto mkdir      = cpp::c08{"mkdir \""}.strcat({path, "\""}, [&](uxx length){
            auto ptr    = buf;

            if (length >= sizeof(buf) - 1){
                ptr     = cpp::alloc<char>(cpp::memory_size{length + 1});
            }

            ptr[length] = '\0';
            return ptr;
        });

        system((asciis)mkdir);

        if (mkdir.operator char *() != buf){
            cpp::free<char>(mkdir, cpp::memory_size{mkdir.length() + 1});
        }

        #elif xis_linux

        auto && buf     = cpp::path_buffer{};
        auto    target  = buf.alloc(the.path);
        dir_operator("/bin/mkdir", "mkdir", "-p", target);
        buf.free(target, path);

        #else
        #error "os miss match"
        #endif
    }

    void dir::as_cwd() const {
        auto && buf     = cpp::path_buffer{};
        auto    target  = buf.alloc(the.path);

        #if xis_windows
        ::SetCurrentDirectoryA(target);
        #elif xis_linux
        ::chdir(target);
        #endif
        buf.free(target, path);
    }
}

#endif
