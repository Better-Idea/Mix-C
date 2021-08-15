#ifndef xpack_macro_init_list
#define xpack_macro_init_list
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_init_list::inc
#include"configure/init_order.hpp"
#include"define/base_type.hpp"
#include"macro/xcstyle.hpp"
#include"macro/xexport.hpp"
#include"macro/xlink.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_init_list{
    using invoke_t = void(*)();
    struct init_list;

    inline init_list * g_init_list[inc::the_end];

    struct init_list{
        init_list * next;       // 平级节点
        invoke_t    invoke;

        init_list(uxx priority, invoke_t invoke) : 
            next(nullptr),
            invoke(invoke){

            this->next              = g_init_list[priority];
            g_init_list[priority]   = this;
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
        for(uxx i = 0; i < inc::the_end; i++){
            for(auto current = g_init_list[i]; current != nullptr; current = current->next){
                current->invoke();
            }
        }
    }
}

// 只允许在全局添加初始化函数
// 实际初始化会被推迟到 main 中执行
// 注意：这里使用 __LINE__ 而不是 __COUNTER__ 避免在不同的 .cpp 编译目录下出现不同的副本
#define xinit(...)                                              \
    inline ::mixc::macro_init_list::init_listx<__VA_ARGS__>     \
    xlink2(__init_list, __LINE__) = xcstyle() -> void
#endif

xexport_space(mixc::macro_init_list::origin)

// 初始化顺序
#include"configure/init_order.hpp"