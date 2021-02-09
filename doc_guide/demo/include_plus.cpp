#define xuser mixc::powerful_cat
#include"configure.hpp"
#include"interface/seqptr.hpp"
#include"io/dir.hpp"
#include"io/file.hpp"
#include"io/tty.hpp"
#include"lang/cxx/compare_fastly.hpp"
#include"lang/cxx/index_of_suffix.hpp"
#include"lang/cxx/ph.hpp"
#include"lang/cxx.hpp"
#include"mixc.hpp"

namespace xuser{
    void foreach(c08 const & root, seqptr<c08> folders){
        folders.foreach([&](c08 folder){
            tty.write_line(folder);
            dir{root}.as_cwd();
            dir{folder}.as_cwd();

            file plus{"+.hpp.tmp"};
            plus.remove();

            dir{"."}.subfiles().foreach([&](asciis name){
                using namespace ph;

                c08 header_file = name;

                if (header_file == "+.hpp" or
                    header_file.index_of_suffix({".hpp"}) == not_exist){
                    return;
                }

                c08 line = v{ folder, header_file }.format("#include\"\v\v\"" xlocal_endl, [](uxx length){
                    static char buf[256];
                    buf[length] = '\0';
                    return buf;
                });
                plus.write(asciis(line), line.length());
            });

            plus.close();
            plus.move_to_forcedly("+.hpp");
        });
    }
}

int main(int argc, asciis * argv){
    if (argc != 2){
        return -1;
    }

    xuser::foreach(argv[1], {
        "algo/",
        "chrono/",
        "define/",
        "docker/",
        "dumb/",
        "instruction/",
        "io/",
        "lang/",
        "lang/cxx/",
        "lang/wxx/",
        "lock/",
        "macro/",
        "math/",
        "math/distribution/",
        "math/statistics/",
        "memop/",
        "meta/",
        "meta_seq/",
        "utils/",
        "test/algo/",
    });
    return 0;
}