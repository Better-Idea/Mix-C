#ifndef xpack_macro_init_list
#define xpack_macro_init_list
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_init_list::inc
#include"define/base_type.hpp"
#include"macro/xcstyle.hpp"
#include"macro/xexport.hpp"
#include"macro/xlink.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_init_list{
    using invoke_t = void(*)();
    struct init_list;

    inline init_list * top              = nullptr;

    struct init_list{
        init_list * next_level; // 更低优先级节点
        init_list * next_item;  // 平级节点
        uxx         priority;   // 值越大优先级越低
        invoke_t    invoke;

        init_list(uxx priority, invoke_t invoke) : 
            next_level(nullptr),
            next_item(nullptr),
            priority(priority),
            invoke(invoke){
            
            if (top == nullptr){
                top                     = this;
                return;
            }

            init_list * current         = top;
            init_list * next            = top->next_level;
            
            while(priority > current->priority){
                if (next == nullptr or priority < next->priority){
                    this->next_level    = current->next_level;
                    current->next_level = this;
                    return;
                }
                if (priority == next->priority){
                    this->next_item     = current;
                    this->next_level    = current->next_level;
                    return;
                }

                current                 = next;
                next                    = next->next_level;
            }

            if (priority == top->priority){
                this->next_item         = top;
                this->next_level        = top->next_level;
                top                     = this;
            }
            // priority < top->priority
            else{
                this->next_level        = top;
                top                     = this;
            }
        }
    };

    template<uxx priority_v = 64>
    struct init_listx : init_list{
        init_listx(invoke_t invoke) :
            init_list(priority_v, invoke){
        }
    };
}

namespace mixc::macro_init_list::origin{
    using mixc::macro_init_list::init_list;
    using mixc::macro_init_list::init_listx;

    inline void init_list_execute(){
        for(init_list * head = top; head != nullptr; head = head->next_level){
            for(init_list * current = head; current != nullptr; current = current->next_item){
                current->invoke();
            }
        }
    }
}

// 只允许在全局添加初始化函数
// 实际初始化会被推迟到 main 中执行
#define xinit(...)                                              \
    inline ::mixc::macro_init_list::init_listx<__VA_ARGS__>     \
    xlink2(__init_list, __COUNTER__) = xcstyle() -> void
#endif

xexport_space(mixc::macro_init_list::origin)

// 初始化顺序
#include"configure/init_order.hpp"