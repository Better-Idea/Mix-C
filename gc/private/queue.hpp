#ifndef xpack_gc_queue
#define xpack_gc_queue
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_queue::inc
#include"interface/can_callback.hpp"
#include"concurrency/lock/atom_fetch_add.hpp"
#include"concurrency/lock/atom_swap.hpp"
#include"memory/allocator.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_queue{
    using release_callback = inc::icallback<void()>;

    struct queue{
        struct node{
            release_callback exec;
            node *           next;

            node() : 
                next(nullptr){
            }
        };

        static constexpr uxx              buffer_size = 1024 * 64;
        static constexpr uxx              mask_index  = buffer_size - 1;
        static constexpr uxx              step        = 1;
        static inline    uxx              end         = 0;
        static inline    uxx              begin       = 0;
        static inline    node *           first       = inc::alloc_with_initial<node>();
        static inline    node *           expand      = first;
        static inline    release_callback release_list[buffer_size];

        // 前台多线程 并发/并行 推送
        static void push(release_callback item) {
            static_assert((mask_index & buffer_size) == 0);
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
            static_assert((mask_index & buffer_size) == 0);
            auto current = begin++;
            auto index   = current & mask_index;

            if (release_list[index] != nullptr){
                release_list[index]();
                release_list[index] = nullptr;
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

xexport(mixc::gc_queue::queue)