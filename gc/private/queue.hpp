#ifndef xpack_gc_queue
#define xpack_gc_queue
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::gc_queue
        #include"define/base_type.hpp"
        #include"interface/can_callback.hpp"
        #include"lock/atom_fetch_add.hpp"
        #include"lock/atom_swap.hpp"
        #include"macro/xvolatile.hpp"
        #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::gc_queue{
        using release_callback = inc::can_callback<void()>;

        struct queue{
            static constexpr uxx buffer_size = 1024 * 64;
            static constexpr uxx mask_index  = buffer_size - 1;
            static constexpr uxx step        = 1;
            static inline uxx    end         = 0;
            static inline uxx    begin       = 0;
            static inline release_callback release_list[buffer_size];

            struct node{
                release_callback exec;
                node *           next;

                node() : 
                    next(nullptr){
                }
            };

            static inline node * first  = inc::alloc_with_initial<node>();
            static inline node * expand = first;

            // 前台多线程 并发/并行 推送
            static void push(release_callback item) {
                auto   index = inc::atom_fetch_add(& end, step) & mask_index;
                auto & cur   = release_list[index];

                // 使用堆内存
                if (cur == nullptr){
                    auto new_item = inc::alloc_with_initial<node>();
                    auto current  = inc::atom_swap(& expand, new_item);
                    current->exec = item;
                    current->next = new_item;
                }
                // 复用静态内存
                else{
                    cur = item;
                }
            }

            // 后台单线程 异步 处理
            static void pop() {
                if (auto index = begin & mask_index; release_list[index] != nullptr){
                    release_list[index]();
                    release_list[index] = nullptr;
                    begin += 1;
                }
                else if (first->exec != nullptr){
                    auto cur = first;
                    first->exec();
                    first = first->next;
                    inc::free(cur);
                }
            }
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::gc_queue::queue;
    using ::mixc::gc_queue::release_callback;
}
