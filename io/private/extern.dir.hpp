#undef  xuser
#define xuser mixc::io_private_dir::inc
#include"configure/switch.hpp"
#include"io/dir.hpp"
#include"io/private/path_buffer.hpp"
#include"lang/cxx/compare_fastly.hpp"
#include"lang/cxx/is_contains.hpp"
#include"lang/cxx/strcat.hpp"
#include"lang/cxx.hpp"
#include"macro/xdefer.hpp"
#include"macro/xref.hpp"
#include"macro/xwarning.hpp"
#include"memop/copy.hpp"
#include"utils/memory.hpp"

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
#include"cmd.hpp"
#endif

xwarning_disable(26444)

namespace mixc::io_dir::origin{
    bstate_t dir::remove() const{
        auto && buf         = cpp::path_buffer{};
        auto    source      = buf.alloc(the.m_path);
        #if xis_windows
        auto    is_success  = ::RemoveDirectoryA(asciis(source)) ?
            bstate_t::success : bstate_t::fail;

        #elif xis_linux
        auto    is_success  = cmd("/bin/rm", "rm", "-rf", source);
        #else
        #error "os miss match"
        #endif

        buf.free(source, m_path);
        return is_success;
    }

    bstate_t dir::move_to(inc::c08 new_path) const{
        auto && buf         = cpp::path_buffer{};
        auto    source      = buf.alloc(the.m_path);
        auto    target      = buf.alloc(new_path);

        #if xis_windows
        auto && opr         = SHFILEOPSTRUCTA{};
        opr.wFunc           = FO_MOVE;
        opr.pFrom           = (asciis)source;
        opr.pTo             = (asciis)target;
        opr.fFlags          = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
        auto is_success     = SHFileOperationA(& opr) == 0 ?
            bstate_t::success : bstate_t::fail;

        #elif xis_linux
        auto is_success     = cmd("/bin/mv", "mv", "-r", source, target);
        #else
        #error "os miss match"
        #endif

        buf.free(target, new_path);
        buf.free(source, the.m_path);
        return is_success;
    }

    bstate_t dir::copy_to(inc::c08 new_path) const{
        auto && buf         = cpp::path_buffer{};
        auto    source      = buf.alloc(the.m_path);
        auto    target      = buf.alloc(new_path);

        #if xis_windows
        auto && opr         = SHFILEOPSTRUCTA{};
        opr.wFunc           = FO_COPY;
        opr.pFrom           = (asciis)source;
        opr.pTo             = (asciis)target;
        opr.fFlags          = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
        auto is_success     = SHFileOperationA(& opr) == 0 ?
            bstate_t::success : bstate_t::fail;

        #elif xis_linux
        auto is_success     = cmd("/bin/cp", "cp", "-r", source, target);
        #else
        #error "os miss match"
        #endif

        buf.free(target, new_path);
        buf.free(source, the.m_path);
        return is_success;
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
        })(path, xref(entry));

        if (fd == INVALID_HANDLE_VALUE){
            return;
        }

        xdefer{
            ::CloseHandle(fd);
        };

        #define xentry_name     (entry.cFileName)
        #define xis_dir         (0 != (entry.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        #define xhas_next       (FindNextFileA(fd, xref(entry)) != 0)

        #elif xis_linux
        struct
        dirent  * entry;
        DIR     * fd;
        auto &&   buf       = cpp::path_buffer{};
        auto      source    = buf.alloc(path);

        if (fd = opendir(asciis(source)); fd == nullptr){
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

    bstate_t dir::create() const {
        #if xis_windows
        // 防止注入攻击
        if (cpp::c08{m_path}.is_contains('\"')){
            return bstate_t::fail;
        }

        char buf[256];
        auto mkdir          = cpp::c08{"mkdir \""}.strcat({m_path, "\""}, [&](uxx length){
            auto ptr        = buf;

            if (length >= sizeof(buf) - 1){
                ptr         = cpp::memory::alloc<char>(cpp::memory::size{length + 1});
            }

            ptr[length]     = '\0';
            return ptr;
        });

        auto is_success     = system((asciis)mkdir) == 0 ?
            bstate_t::success : bstate_t::fail;

        if (mkdir.operator char *() != buf){
            cpp::memory::free<char>((char *)mkdir, cpp::memory::size{mkdir.length() + 1});
        }

        #elif xis_linux
        auto && buf         = cpp::path_buffer{};
        auto    target      = buf.alloc(the.m_path);
        auto    is_success  = cmd("/bin/mkdir", "mkdir", "-p", target);
        buf.free(target, m_path);

        #else
        #error "os miss match"
        #endif

        return is_success;
    }

    bstate_t dir::as_cwd() const {
        auto && buf         = cpp::path_buffer{};
        auto    target      = buf.alloc(the.m_path);

        #if xis_windows
        auto    is_success  = ::SetCurrentDirectoryA((asciis)target) ?
            bstate_t::success : bstate_t::fail;

        #elif xis_linux
        auto    is_success  = ::chdir(asciis(target)) == 0 ?
            bstate_t::success : bstate_t::fail;

        #endif

        buf.free(target, m_path);
        return is_success;
    }
    
    bool dir::is_exist() const {
        auto && buf         = cpp::path_buffer{};
        auto    source      = buf.alloc(the.m_path);

        #if xis_windows
        auto    attr        = GetFileAttributesA((asciis)source);
        auto    exist       = INVALID_FILE_ATTRIBUTES != attr and 0 != (attr & FILE_ATTRIBUTE_DIRECTORY);

        #elif xis_linux

        typedef struct stat meta_t;
        auto && meta        = meta_t{};
        auto    exist       = stat(asciis(source), & meta) != -1 and (meta.st_mode & S_IFDIR) != 0;

        #else
        #error "os miss match"
        #endif

        buf.free(source, m_path);
        return  exist;
    }
}
