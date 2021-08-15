#define xuser mixc::powerful_cat
#include"io/dir.hpp"
#include"io/file.hpp"
#include"io/tty.hpp"
#include"lang/cxx/count_of.hpp"
#include"lang/cxx/index_of_suffix.hpp"
#include"lang/cxx.hpp"
#include"mixc.hpp"

char buf[16 * 1024 * 1024];
char path[1024];
uxx  lines;

namespace xuser{
    uxx i = 0;
    void foreach(){
        dir{"."}.subdirs().foreach([](asciis folder){
            dir{folder}.as_cwd();
            foreach();
            dir{".."}.as_cwd();
        });

        dir{"."}.subfiles().foreach([](asciis source){
            if (c08{source}.index_of_suffix({".hpp",".h",".cpp"}) == not_exist){
                return;
            }
            tty.write_line(i++);
            uxx bytes = file{source}.read(buf, sizeof(buf));
            lines += c08{buf, bytes}.count_of('\n');
        });
    }
}


xinit(xuser::the_main){
    using namespace xuser;
    tty.write_line("please input the mix-c project full root path");
    tty.read_line([](uxx length){
        if (length >= sizeof(path)){
            throw;
        }
        path[length] = '\0';
        return path;
    });

    xhint(path);
    xuser::dir{path}.as_cwd();
    xuser::foreach();
    xhint(lines);
};