#define xuser mixc::none
// #define xenable_xdebug

//#include"macro/xgc.hpp"
#include"gc/private/ref.hpp"
#include"docker/hashmap.hpp"
#include"define/nullref.hpp"
#include"define/nan.hpp"
#include"docker/transmitter.hpp"
#include"macro/xgc.hpp"

namespace xuser {
    struct bx {
        xgc_fields(
            xiam(bx),
            xpri(data, int),
            xpri(is_none, mutable bool)
        )

        void hi() {
        }
    };

    void entry() {
        //auto & p = inc::nullref;
    }
}

#include<stdio.h>
int main() {
    return 0;
}
