#ifndef xpack_gc_private_background
#define xpack_gc_private_background
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_private_background::inc
#include"define/base_type.hpp"
#include"concurrency/lock/atom_fetch_add.hpp"
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_store.hpp"
#include"concurrency/lock/atom_fetch_or.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/lock/cache_load.hpp"
#include"concurrency/lock/cache_store.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"configure/buffer.hpp"
#include"gc/private/token.hpp"
#include"macro/xcheck_binary_aligned.hpp"
#include"macro/xdefer.hpp"
#include"macro/xexport.hpp"
#include"macro/xinit.hpp"
#include"macro/xref.hpp"
#include"utils/memory.hpp"

#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_private_background::origin{
    using release_invoke    = void(*)(voidp mem);

    struct release_pair{
        inc::token *        mem;
        release_invoke      release;
    };

    constexpr uxx gc_filter_queue_depth_mask    = xgc_filter_queue_depth - 1;
    constexpr uxx gc_free_list_queue_depth_mask = xgc_free_list_queue_depth - 1;
    xcheck_binary_aligned(xgc_filter_queue_depth);
    xcheck_binary_aligned(xgc_free_list_queue_depth);

    using free_nodep            = inc::token::free_nodep;
    using release_pairp         = release_pair *;

    // 控制 g_gc_filter_thread 中的[内析构]不再推送给自己，而是直接执行
    // 该值只会在 g_gc_filter_thread 中设置，其他线程读到的都是 false
    inline thread_local bool    l_in_release;
    inline thread_local voidp   l_wait_mem;
    inline thread_local uxx     l_i_wait;

    // 等待 GC 同步的线程数
    inline uxx                  g_pending;

    // gc 对象筛选队列
    inline release_pair         gc_filter_queue[xgc_filter_queue_depth];

    // gc 释放列表
    inline free_nodep           g_free_list;

    // 存放索引，当前推送的内容存放到 gc_filter_queue 的哪一个元素中
    inline uxx                  g_i_push_gc_que;

    // 取出索引，gc_thread 处理了多少 gc_filter_queue 中的请求
    inline uxx                  g_i_pop_gc_que;

    inline uxx                  g_gc_exit;

    inline free_nodep           g_free_list_queue[xgc_free_list_queue_depth];

    // gc 退出触发器，当全局析构时触发
    xdefer_global{
        for(inc::atom_store(xref(g_gc_exit), true);
            inc::atom_load(xref(g_gc_exit)); 
            inc::thread_self::yield()){
        }
    };

    inline inc::thread          g_gc_filter_thread;
    inline inc::thread          g_gc_release_thread;

    inline void gc_execute(){
        for(auto i_queue = 0; inc::atom_load(xref(g_gc_exit)) == false;){
            auto i_push         = inc::atom_load(xref(g_i_push_gc_que));
            auto i_pop          = g_i_pop_gc_que;
            auto dis            = i_push - g_i_pop_gc_que;

            if (dis >= xgc_filter_queue_depth) {
                i_push          = g_i_pop_gc_que + xgc_filter_queue_depth;
            }

            if (dis == 0){
                inc::thread_self::suspend(64/*ms*/);
                continue;
            }

            // 等待超时就响应一下
            if (inc::atom_load(xref(g_pending)) == 0 and dis < xgc_queue_threshold){
                inc::thread_self::suspend(64/*ms*/);
            }

            using tp            = inc::token *;
            using rpp           = release_pair *;
            auto i_begin        = i_pop;
            auto i_end          = i_push;
            auto i              = i_push;
            auto release        = release_invoke{};
            auto mem            = tp{};
            auto item           = rpp{};
            auto current        = free_nodep{};

            // 去除重复的析构（mem 指向的值一样）
            // 相同指向的 mem 只保留最后的一个，保证释放的顺序，所以循环是从后往前
            // 批量让引用计数器减一
            while(i != i_begin){
                i              -= 1;
                item            = xref(gc_filter_queue[i & gc_filter_queue_depth_mask]);

                // mem 为 nullptr 指示当前元素在对应的线程中还未完成设置
                while(true){
                    if (mem = inc::atom_load(xref(item->mem)); mem != nullptr){
                        // 加载一次 release 让当前 cpu 可见
                        release = inc::atom_load(xref(item->release));
                        mem->owners_decrease();
                        break;
                    }
                    inc::thread_self::yield();
                }

                if (mem->in_gc_queue()){
                    item->mem   = nullptr;
                }
                else{
                    mem->in_gc_queue(true);
                }
            }

            // 可能是来自 push 达到阈值 xgc_queue_threshold 时唤醒
            // 所以被唤醒时，仍以当前队列是否空闲作为标志
            while(true){
                if (g_free_list = inc::atom_load(xref(g_free_list_queue[i_queue])); 
                    g_free_list == nullptr){
                    break;
                }
                inc::thread_self::suspend();
            }

            for(inc::token::new_term(); i != i_end;) {
                item            = xref(gc_filter_queue[i & gc_filter_queue_depth_mask]);
                i              += 1;
                i_pop          += 1;
                release         = item->release;
                mem             = item->mem;

                // 不执行重复无效的遍历
                // 只有该节点是在本轮 gc 中第一次访问，才可以进行接下来的[外析构]
                if (mem and not mem->is_visited() and not mem->under_release()){
                    release(mem);
                }

                // 该对象可能在 gc 遍历其他对象时已经经过了
                // 所以我们不能将 mem->in_gc_queue(false) 放到 release(mem) 后边，而是单独作为一个 if
                // 对于该对象仍活跃的情况我们必须复位 in_gc_queue 以便一下次送入 gc 队列时不被误判
                if (mem){
                    mem->in_gc_queue(false);
                }

                // 需要先置空，再设置 g_i_pop_gc_que
                // 只有下一次读到 mem 值不为 nullptr 时才响应
                inc::atom_store(xref(item->mem), nullptr);

                // 让 gc_filter_queue 立即腾出可用的空间
                inc::atom_store(xref(g_i_pop_gc_que), i_pop);
            }

            if (g_free_list){
                inc::cache_store();
                inc::atom_store(xref(g_free_list_queue[i_queue]), g_free_list);
                i_queue        += 1;
                i_queue        &= gc_free_list_queue_depth_mask;
                g_gc_release_thread.resume();
            }
        }
        g_gc_release_thread.resume();
    }

    inline void gc_sync(){
        inc::atom_add(xref(g_pending), 1);
        g_gc_filter_thread.resume();

        // 如果 gc 还未处理完该 mem 就自旋等待
        while(inc::atom_load(xref(gc_filter_queue[l_i_wait].mem)) == l_wait_mem){
            inc::thread_self::yield();
        }
        inc::atom_sub(xref(g_pending), 1);
    }

    // 注意：mem 不能为 nullptr
    inline void gc_push(inc::token * mem, release_invoke release){
        uxx i_push              = inc::atom_fetch_add(xref(g_i_push_gc_que), 1);
        uxx i_pop               = 0;
        uxx i                   = 0;
        uxx dis                 = 0;

        while(true){
            i_pop               = inc::atom_load(xref(g_i_pop_gc_que));
            i                   = i_push & gc_filter_queue_depth_mask;
            dis                 = i_push - i_pop;

            if (dis < xgc_filter_queue_depth){
                break;
            }
            else{
                inc::thread_self::yield();
                continue;
            }
        }

        // 设置当前等待 gc 完成的元素
        l_i_wait                = i;
        l_wait_mem              = mem;

        // 后设置 mem，该字段不为 nullptr 指示当前元素设置完成
        inc::atom_store(xref(gc_filter_queue[i].release), release);
        inc::atom_store(xref(gc_filter_queue[i].mem), mem);

        if (i == xgc_queue_threshold){
            g_gc_filter_thread.resume();
        }
    }

    xinit(inc::the_gc_filter){
        uxx size_of_filter      = xgc_filter_queue_depth * sizeof(release_pair);

        g_gc_filter_thread      = inc::thread(xdetached{
            gc_execute();
        });
    };

    xinit(inc::the_gc_release){
        g_gc_release_thread     = inc::thread(xdetached{
            for(uxx i_queue = 0;;){
                auto free_list  = free_nodep{};
                auto current    = free_nodep{};

                // 如果遇到退出标志，可以立即退出，因为纯内存释放操作可以不用在系统退出时执行
                if (free_list = inc::atom_swap(xref(g_free_list_queue[i_queue]), free_list); 
                    free_list == nullptr){
                    if (inc::thread_self::suspend();
                        inc::atom_load(xref(g_gc_exit))){
                        break;
                    }
                }

                inc::cache_load();
                g_gc_filter_thread.resume();
                i_queue        += 1;
                i_queue        &= gc_free_list_queue_depth_mask;

                // 然后做释放内存等耗时操作
                while(free_list != nullptr){
                    current     = free_list;
                    free_list   = free_list->prev();
                    inc::memory::free(current, inc::memory::size{current->bytes()});
                }
            }

            // 回应一下主线程，退出以及完成
            inc::atom_store(xref(g_gc_exit), false);
        });
    };
}

#endif

xexport_space(mixc::gc_private_background::origin)