#undef  xuser
#define xuser mixc::talk_about_policy_barrier::inc
#include"concurrency/lock/mutex.hpp"
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/policy_barrier.hpp"
#include"concurrency/thread.hpp"
#include"macro/xhint.hpp"
#include"mixc.hpp"

namespace xuser{
    // 操作列表
    enum opr{
        push,
        pop
    };

    using barrier_t = policy_barrier<
        // 我们在 moudle::push 中使用了原子操作，所以 push 与 push 间是可以并发的
        when<push>::can<push>,

        // 而 pop 是独占的
        when<pop>::can<>
    >;

    struct foo{
        barrier_t barrier;
        uxx       counter = 0;

        void push(){
            barrier.lock<opr::push>([&](){
                atom_add<uxx>(xref(counter), 1);
            });
        }

        bool pop(){
            bool has_value = false;
            barrier.lock<opr::pop>([&](){
                if (counter != 0){
                    counter  -= 1;
                    has_value = true;
                }
            });
            return has_value;
        }
    };
}

xinit(xuser::the_main){
    using namespace xuser;
    uxx     step = 1000000;
    uxx     sum  = step * 4;
    foo     mod;

    auto push_invoke = [&](voidp id){
        xhint("push", id);
        for(uxx i = 0; i < step; i++){
            mod.push();
        }
        xhint("finish push", id);
    };

    auto pop_invoke = [&](voidp id){
        xhint("pop", id);
        for(uxx i = 0; i < step * 2;){
            if (mod.pop()){
                i++;
            }
        }
        xhint("finish pop", id);
    };

    {
        thread list[] = {
            xjoinable{ push_invoke(voidp(0)); },
            xjoinable{ push_invoke(voidp(1)); },
            xjoinable{ push_invoke(voidp(2)); },
            xjoinable{ push_invoke(voidp(3)); },
            xjoinable{ pop_invoke (voidp(4)); },
            xjoinable{ pop_invoke (voidp(5)); },
        };
    }

    xhint(mod.counter);
};
