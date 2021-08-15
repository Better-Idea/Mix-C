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
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"configure/buffer.hpp"
#include"gc/private/token.hpp"
#include"macro/xdefer.hpp"
#include"macro/xexport.hpp"
#include"macro/xinit.hpp"
#include"macro/xref.hpp"
#include"utils/memory.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_private_background::origin{
    using release_invoke    = void(*)(voidp mem);

    struct release_pair{
        inc::token *        mem;
        release_invoke      release;
    };

    constexpr uxx gc_queue_depth_mask   = xgc_queue_depth - 1;
    static_assert((gc_queue_depth_mask & ~gc_queue_depth_mask) == 0, "gc_queue_depth_mask not aligned");

    // 控制 g_gc_filter_thread 中的[内析构]不再推送给自己，而是直接执行
    // 该值只会在 g_gc_filter_thread 中设置，其他线程读到的都是 false
    inline thread_local bool    l_in_release;
    inline thread_local voidp   l_wait_mem;
    inline thread_local uxx     l_i_wait;

    using free_nodep = inc::token::free_nodep;
    using release_pairp = release_pair *;

    // 释放列表，先添加到列表中
    inline free_nodep           g_free_list;

    // 等待 GC 同步的线程数
    inline uxx                  g_pending;

    // gc 缓冲队列
    // 前台推送，后台释放
    inline release_pair *       g_gc_que;

    // 存放索引，当前推送的内容存放到 g_gc_que 的哪一个元素中
    inline uxx                  g_i_push_gc_que;

    // 取出索引，gc_thread 处理了多少 g_gc_que 中的请求
    inline uxx                  g_i_pop_gc_que;

    inline uxx                  g_gc_exit;

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
        while(inc::atom_load(xref(g_gc_exit)) == false){
            auto i_push         = inc::atom_load(xref(g_i_push_gc_que));
            auto i_pop          = g_i_pop_gc_que;
            auto dis            = i_push - g_i_pop_gc_que;

            if (dis >= xgc_queue_depth) {
                i_push          = g_i_pop_gc_que + xgc_queue_depth;
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
                item            = xref(g_gc_que[i & gc_queue_depth_mask]);

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

            // 如果 g_gc_release_thread 还未接收上一个 g_free_list，这里就等待一下
            // 大部分时候 g_gc_release_thread 的任务都比 g_gc_filter_thread 轻松且更快完成
            // 所以很少有 filter 等待 release 的情况
            if (g_free_list = inc::atom_load(xref(g_free_list)); g_free_list){
                inc::thread_self::suspend();
            }

            for(inc::token::new_term(); i != i_end;) {
                item            = xref(g_gc_que[i & gc_queue_depth_mask]);
                i              += 1;
                i_pop          += 1;
                release         = item->release;
                mem             = item->mem;

                // 不执行重复无效的遍历
                // 只有该节点是在本轮 gc 中第一次访问，才可以进行接下来的[外析构]
                if (mem and not mem->is_visited() and not mem->under_release()){
                    release(mem);
                }

                if (mem) {
                    mem->in_gc_queue(false);
                }

                // 需要先置空，再设置 g_i_pop_gc_que
                // 只有下一次读到 mem 值不为 nullptr 时才响应
                inc::atom_store(xref(item->mem), nullptr);

                // 让 g_gc_que 立即腾出可用的空间
                inc::atom_store(xref(g_i_pop_gc_que), i_pop);
            }

            inc::atom_store(xref(g_free_list), g_free_list);
            g_gc_release_thread.resume();
        }
        g_gc_release_thread.resume();
    }

    inline void gc_sync(){
        inc::atom_add(xref(g_pending), 1);
        g_gc_filter_thread.resume();

        // 如果 gc 还未处理完该 mem 就自旋等待
        while(inc::atom_load(xref(g_gc_que[l_i_wait].mem)) == l_wait_mem){
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
            i                   = i_push & gc_queue_depth_mask;
            dis                 = i_push - i_pop;

            if (dis < xgc_queue_depth){
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
        inc::atom_store(xref(g_gc_que[i].release), release);
        inc::atom_store(xref(g_gc_que[i].mem), mem);

        if (i == xgc_queue_threshold){
            g_gc_filter_thread.resume();
        }
    }

    xinit(inc::the_gc_filter){
        uxx bytes               = xgc_queue_depth * sizeof(release_pair);
        g_gc_que                = release_pairp(inc::memory::malloc(bytes));

        for(uxx i = 0; i < xgc_queue_depth; i++){
            xnew(& g_gc_que[i]) release_pair();
        }

        g_gc_filter_thread      = inc::thread(xdetached{
            gc_execute();
        });
    };

    xinit(inc::the_gc_release){
        g_gc_release_thread     = inc::thread(xdetached{
            while(true){
                // 如果遇到退出标志，可以立即退出，因为纯内存释放操作可以不用在系统退出时执行
                if (inc::thread_self::suspend();
                    inc::atom_load(xref(g_gc_exit))){
                    break;
                }

                auto free_list  = free_nodep{};
                auto current    = free_nodep{};
                free_list       = inc::atom_swap(xref(g_free_list), free_list);
                g_gc_filter_thread.resume();

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