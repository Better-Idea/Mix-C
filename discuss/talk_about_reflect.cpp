#define xuser mixc::talk_about_reflect::inc
#include"io/tty.hpp"
#include"macro/xtypeid.hpp"
#include"mixc.hpp"

xstruct(
    xname(bar),
    xpubf(sex, char)
) $

xstruct(
    xname(foo),
    xpubb(bar),
    xpubf(age, i32),
    xpubf(name, asciis)
) $

int run(){
    foo f;
    f.sex   = 'M';
    f.age   = 24;
    f.name  = "cat";

    xtypeid(foo).foreach_fields(f, [](uxx i, asciis name, auto const & value){
        xuser::tty.write_line(name, ':', value);
    });
    return 0;
}
