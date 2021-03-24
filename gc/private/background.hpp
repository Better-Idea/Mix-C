#ifndef xpack_gc_private_background
#define xpack_gc_private_background
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::gc_private_background::inc
#include"concurrency/lock/atom_fetch_add.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_store.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"configure.hpp"
#include"docker/hashmap.hpp"
#include"gc/private/token.hpp"
#include"macro/xexport.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::gc_private_background::origin{
    xstruct(
        xname(gc_info_t)
    )
        uxx can_arrive_root : 1;
        uxx visited         : sizeof(uxx) * 8 - 1;

        gc_info_t() : 
            can_arrive_root(0), visited(0){}

        operator uxx (){
            return uxxp(this)[0];
        }
    $

    using visited_ptr_t     = voidp;
    using gc_map_t          = inc::hashmap<visited_ptr_t, gc_info_t>;
    using release_invoke    = void(*)(voidp mem);

    xstruct(
        xname(release_pair)
    )
        inc::token *        mem;
        release_invoke      release;
    $

    xstruct(
        xname(free_node)
    )
        free_node *         previous;
        inc::memory_size    bytes;
    };

    xstruct(
        xname(pending_node)
    )
        pending_node *      next;
        uxx                 i_wait;
        uxx                 thread_id;
    $

    using free_nodep            = free_node *;
    using pending_nodep         = pending_node *;

    constexpr uxx gc_queue_depth_mask   = xgc_queue_depth - 1;
    constexpr uxx gc_queue_threshold    = xgc_queue_depth / 32;
    static_assert((gc_queue_depth_mask & ~gc_queue_depth_mask) == 0, "gc_queue_depth_mask not aligned");

    // 控制 gc_thread 中的[内析构]不再推送给自己，而是直接执行
    // 该值只会在 gc_thread 中设置，其他线程读到的都是 false
    inline thread_local bool   in_release;

    // 保存 gc 经过的节点
    inline gc_map_t            gc_map;

    // 当前[潜质类型]的出度入度差
    // 如果该[潜质类型]可以通过子节点再次回到自身且 degree_dvalue 为 0 则表示可以进行[内析构]
    inline uxx                 degree_dvalue;

    // gc 遍历时的根节点
    inline visited_ptr_t       root;

    // 释放列表，先添加到列表中，后批量释放
    inline free_nodep          free_list;

    // gc 缓冲队列
    // 前台推送，后台释放
    inline release_pair        gc_que[xgc_queue_depth];

    // 存放索引，当前推送的内容存放到 gc_que 的哪一个元素中
    inline uxx                 i_push_gc_que;

    // 取出索引，gc_thread 处理了多少 gc_que 中的请求
    inline uxx                 i_pop_gc_que;

    inline pending_node        gc_pend_temp;
    inline pending_nodep       gc_pend_head  = xref gc_pend_temp;
    inline pending_nodep       gc_pend       = gc_pend_head;
    inline inc::thread         gc_thread;

    inline void gc_execute(){
        while(true){
            auto i_push             = inc::atom_load(xref i_push_gc_que);
            auto i_pop              = i_pop_gc_que;
            auto dis                = i_push - i_pop_gc_que;
            auto first              = pending_nodep{};
            auto next               = pending_nodep{};
            auto resume             = [&](){
                for(uxx id;;){
                    if (first == gc_pend_head or first == nullptr){
                        first       = inc::atom_load(xref gc_pend_head->next); 
                    }
                    if (first == nullptr or i_pop < inc::atom_load(xref first->i_wait)){
                        break;
                    }
                    if (next = inc::atom_load(xref first->next); next != nullptr){
                        id          = inc::atom_load(xref first->thread_id);
                        first       = next;
                        inc::thread_self::resume(id);
                        continue;
                    }
                    if (gc_pend_head->next = nullptr, 
                        next = inc::atom_swap(xref gc_pend, gc_pend_head); 
                        next == first){
                        id          = inc::atom_load(xref first->thread_id);
                        first       = nullptr;
                        inc::thread_self::resume(id);
                        break;
                    }
                    else{
                        next->next  = gc_pend_head;
                        break;
                    }
                }
            };

            if (first = inc::atom_load(xref gc_pend_head->next); dis == 0){
                resume();
                inc::thread_self::suspend();
                continue;
            }
            if (first == nullptr and dis < gc_queue_threshold){
                inc::thread_self::suspend(64/*ms*/);
            }

            using tp                = inc::token *;
            using rpp               = release_pair *;
            auto i_begin            = i_pop_gc_que & gc_queue_depth_mask;
            auto i_end              = i_push & gc_queue_depth_mask;
            auto i                  = i_end;
            auto release            = release_invoke{};
            auto mem                = tp{};
            auto item               = rpp{};
            auto current            = free_nodep{};

            // 去除重复的析构（mem 指向的值一样）
            // 批量让引用计数器减一
            while(i != i_begin){
                i                  -= 1;
                item                = xref gc_que[i];

                // mem 为 nullptr 指示当前元素在对应的线程中还未完成设置
                while(true){
                    if (mem = inc::atom_load(xref item->mem); mem != nullptr){
                        // 加载一次 release 让当前 cpu 可见
                        release     = inc::atom_load(xref item->release);
                        mem->owners_decrease();
                        break;
                    }
                    inc::thread_self::yield();
                }

                // 移除重复的析构
                if (mem->in_gc_queue()){
                    item->mem       = nullptr;
                }
                else{
                    mem->in_gc_queue(true);
                }
            }

            while(i != i_end){
                item                = xref gc_que[i];
                i                  += 1;
                i_pop              += 1;
                release             = item->release;
                mem                 = item->mem;

                // 不执行重复无效的遍历
                // 只有该节点是在本轮 gc 中第一次访问，才可以进行接下来的[外析构]
                if (mem and mem->in_gc_queue()){
                    release(mem);

                    // 后设置 in_gc_queue = false
                    // {内析构}会根据 in_gc_queue 是否等于 true 来执行释放操作
                    // 在执行[内析构]时会将 in_gc_queue 置为 false
                    // 如果经过一次 gc 发现 mem 不可以释放([内析构]没有执行)
                    // 则此处需要再复位一下 in_gc_queue，以便下一轮 gc 可以正确响应
                    mem->in_gc_queue(false);

                    // 置为 nulptr
                    // 只有下一次读到 mem 值不为 nullptr 时才响应
                    item->mem       = nullptr;
                }

                // 让 gc_que 立即腾出可用的空间
                inc::atom_store(xref i_pop_gc_que, i_pop);
                resume();
            }

            // 然后做释放内存等耗时操作
            while(free_list != nullptr){
                current             = free_list;
                free_list           = free_list->previous;
                inc::free(current, current->bytes);
            }

            gc_map.clear();
        }
    }

    inline void gc_sync(){
        auto node                   = pending_node{};
        auto current                = xref node;
        auto i_push                 = inc::atom_load(xref i_push_gc_que);
        inc::atom_store(xref current->next, nullptr);
        inc::atom_store(xref current->thread_id, inc::thread_self::id());
        inc::atom_store(xref current->i_wait, i_push);
        auto prev                   = inc::atom_swap(xref gc_pend, current);

        // 先设置 gc_pend 链表，后唤醒 gc_thread，最后挂起自己
        inc::atom_store(xref prev->next, current);
        gc_thread.resume();
        inc::thread_self::suspend();
    }

    // 注意：mem 不能为 nullptr
    inline void gc_push(inc::token * mem, release_invoke release){
        for(auto i_push = inc::atom_fetch_add(xref i_push_gc_que, 1);;){
            auto i_pop              = inc::atom_load(xref i_pop_gc_que);
            auto dis                = i_push - i_pop;

            if (dis < xgc_queue_depth){
                auto i              = i_push & gc_queue_depth_mask;

                // 后设置 mem，该字段不为 nullptr 指示当前元素设置完成
                inc::atom_store(xref gc_que[i].release, release);
                inc::atom_store(xref gc_que[i].mem, mem);

                if (i == gc_queue_threshold){
                    gc_thread.resume();
                }
                return;
            }
            else{
                inc::thread_self::yield();
            }
        }
    }
}

#endif

xexport_space(mixc::gc_private_background::origin)