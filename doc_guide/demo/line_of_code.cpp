#define xuser mixc::powerful_cat
#include"io/dir.hpp"
#include"io/file.hpp"
#include"io/tty.hpp"
#include"lang/cxx/count_of.hpp"
#include"lang/cxx/index_of_suffix.hpp"
#include"lang/cxx.hpp"
#include"mixc.hpp"

char buf[16 * 1024 * 1024];
uxx  lines;

namespace xuser{
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
            uxx bytes = file{source}.read(buf, sizeof(buf));
            lines += c08{buf, bytes}.count_of('\n');
        });
    }
}

int main(i32 argc, asciis argv[]){
    if (argc != 2){
        return -1;
    }

    xuser::dir{argv[1]}.as_cwd();
    xuser::foreach();
    xhint(lines);
    return 0;
}