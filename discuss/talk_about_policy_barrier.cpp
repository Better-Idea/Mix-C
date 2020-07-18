#ifndef xpack_talk_about_policy_barrier
#define xpack_talk_about_policy_barrier
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::talk_about_policy_barrier
#include<thread>
#include"lock/mutex.hpp"
#include"lock/atom_add.hpp"
#include"lock/policy_barrier.hpp"
#include"macro/xhint.hpp"
#pragma pop_macro("xuser")

namespace mixc::talk_about_policy_barrier::inc{
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

    struct module{
        barrier_t barrier;
        uxx       counter = 0;

        void push(){
            barrier.lock<opr::push>([&](){
                atom_add<uxx>(xref counter, 1);
            });
        }

        bool pop(){
            bool has_value = false;
            barrier.lock<opr::pop>([&](){
                if (counter != 0){
                    counter -= 1;
                    has_value = true;
                }
            });
            return has_value;
        }
    };

    void test(){
        using namespace std;
        uxx     step = 1000000;
        uxx     sum  = step * 4;
        module  mod;

        auto push_invoke = [&](voidp id){
            xhint("push", id);
            for(uxx i = 0; i < step; i++){
                mod.push();
            }
            xhint("finish push");
        };

        auto pop_invoke = [&](voidp id){
            xhint("pop", id);
            for(uxx i = 0; i < step * 2;){
                if (mod.pop()){
                    i++;
                }
            }
            xhint("finish pop");
        };
        
        thread list[] = {
            thread(push_invoke, voidp(0)),
            thread(push_invoke, voidp(1)),
            thread(push_invoke, voidp(2)),
            thread(push_invoke, voidp(3)),
            thread(pop_invoke,  voidp(4)),
            thread(pop_invoke,  voidp(5)),
        };

        for(auto & t : list){
            t.join();
        }

        xhint(mod.counter);
    }
}

int main(){
    mixc::talk_about_policy_barrier::inc::test();
    return 0;
}

#endif
