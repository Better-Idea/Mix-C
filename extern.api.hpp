// 内存必须先初始化，可以不按字母顺序包含，该头文件存在 thread_local 变量待初始化
#include"utils/private/extern.allocator.hpp"

#include"concurrency/private/extern.atom.hpp"
#include"concurrency/private/extern.thread.hpp"
#include"concurrency/private/extern.thread_self.hpp"
#include"chrono/private/extern.+.hpp"
#include"instruction/private/extern.isa.hpp"
#include"io/private/extern.dir.hpp"
#include"io/private/extern.file.hpp"
#include"io/private/extern.tty.hpp"
#include"macro/private/extern.log.hpp"
#include"macro/private/extern.xassert.hpp"
#include"math/private/extern.expe.hpp"
#include"math/private/extern.ln.hpp"
#include"math/private/extern.sqrt.hpp"
#include"math/private/extern.trigo+.hpp"

// #include"draft/gui_seed.hpp"

#undef  xuser
#define xuser mixc::powerful_cat
#include"concurrency/thread.hpp"
#include"dumb/move.hpp"
#include"gc/private/background.hpp"
#include"utils/init_list.hpp"

extern int run();

int main(){
    xuser::init_list_execute();
    xuser::gc_thread = xuser::thread(xdetached{
        xuser::gc_execute();
    });

    auto state = run();
    return state;
}
