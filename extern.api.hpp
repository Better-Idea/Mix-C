// 注意：============================================================================
// 如果你没有把握，不要在全局创建异步线程。避免 GC 退出时，异步线程的资源还没有释放
// GC 头文件排到前面，让全局 GC 最后析构，确保其他全局变量都析构完毕，GC 可以安全退出
// 因为 GC 不仅包含内存的释放，也包含资源析构的功能
#define xuser mixc::powerful_cat
#include"concurrency/thread.hpp"
#include"dumb/move.hpp"
#include"gc/private/background.hpp"
#include"utils/init_list.hpp"
#undef  xuser

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
#include"utils/private/extern.allocator.hpp"

// #include"draft/gui_seed.hpp"
#undef  xuser
#define xuser mixc::powerful_cat

extern int run();

int main(){
    xuser::init_list_execute();
    xuser::gc_thread = xuser::thread(xdetached{
        xuser::gc_execute();
    });

    auto state = run();
    return state;
}
