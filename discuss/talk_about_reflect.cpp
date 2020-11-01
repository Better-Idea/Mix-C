#ifndef xpack_talk_about_reflect
#define xpack_talk_about_reflect
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::talk_about_reflect::inc
#include"io/tty.hpp"
#include"macro/xtypeid.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::talk_about_reflect::inc{
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

    void test(){
        foo f;
        f.sex   = 'M';
        f.age   = 24;
        f.name  = "cat";

        xtypeid(foo).foreach_fields(f, [](uxx i, asciis name, auto const & value){
            tty.write_line(name, ':', value);
        });
    }
}

int main(){
    mixc::talk_about_reflect::inc::test();
    return 0;
}

#endif
