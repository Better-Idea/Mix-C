#ifndef xpack_io_private_path_buffer
#define xpack_io_private_path_buffer
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::io_private_path_buffer::inc
#include"define/base_type.hpp"
#include"lang/cxx/clone.hpp"
#include"lang/cxx.hpp"
#include"macro/xexport.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::io_private_path_buffer::origin{
    struct path_buffer{
    private:
        char buf[512];
        char * ptr = buf;
        char * end = buf + sizeof(buf);
    public:
        inc::c08 alloc(inc::c08 path){
            if (path[path.length() - 1] == '\0'){
                return path;
            }
            else{
                return path.clone([&](uxx length){
                    auto p = ptr;

                    if (ptr + length + 1 < end){
                        ptr += length + 1;
                    }
                    else{
                        p = inc::alloc<char>(inc::memory_size{length + 1});
                    }

                    p[length] = '\0';
                    return p;
                });
            }
        }

        void free(inc::c08 path, inc::c08 maybe_ref){
            asciis p = asciis(path);

            if (p == (asciis)maybe_ref){
                return;
            }
            if (buf <= p and p < end){
                return;
            }
            inc::free(voidp(p), inc::memory_size{path.length() + 1});
        }
    };
}

#endif

xexport_space(mixc::io_private_path_buffer::origin)
