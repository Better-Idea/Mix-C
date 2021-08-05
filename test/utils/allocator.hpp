#ifndef xpack_test_utils_allocator
#define xpack_test_utils_allocator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::test_utils_allocator::inc
#include"concurrency/lock/atom_add.hpp"
#include"concurrency/lock/atom_load.hpp"
#include"concurrency/lock/atom_sub.hpp"
#include"concurrency/lock/atom_swap.hpp"
#include"concurrency/thread.hpp"
#include"concurrency/thread_self.hpp"
#include"utils/allocator.hpp"
#include"macro/xassert.hpp"
#include"math/random.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

uxx lut[1024];

namespace mixc::test_utils_allocator{
    constexpr uxx count = 1024 * 1024;
    inline uxxp record[count];
    inline uxx thread_count = 0;

    xtest("allocator"){
        uxx flow_count = 1;

        xhint(inc::used_bytes(), inc::alive_object());
        inc::atom_add(xref(thread_count), flow_count);

        {
            inc::thread trace = xjoinable{
                while (inc::atom_load(xref(thread_count)) ){
                    inc::thread_self::sleep(1000);
                    xhint(inc::used_bytes(), inc::alive_object());
                }
            };

             for(uxx i = 0; i < flow_count; i++){
                 // 并发创建多个线程
                 inc::thread tx = xdetached{
                     for(uxx i = 0; i < count;){
                         auto & r = record[i];
                         auto   alloc = inc::random<bool>();
                         auto   bytes = inc::random<uxx>() % 128 + 1;
                         auto   size = inc::memory_size{ bytes };
                         auto   ptr = alloc ? inc::alloc_with_initial<uxx>(size, bytes) : uxxp(nullptr);
                         auto   mem = inc::atom_swap(xref(r), ptr); 

                         if (mem){
                             inc::free(mem, inc::memory_size{  *mem });
                         }
                         i += inc::random<uxx>() % 32;
                     }

                     inc::atom_sub(xref(thread_count), 1);
                 };
             }

            // 主线程会在 trace 析构时等待
            // trace 会等待所有 tx 线程退出
        }

        for(uxx i = 0; i < count; i++){
            if (auto mem = inc::atom_swap<uxxp>(xref(record[i]), nullptr); mem){
                inc::free(mem, inc::memory_size{ *mem });
            }
        }

        while (true) {
            inc::thread_self::sleep(1000);
            xhint(inc::used_bytes(), inc::alive_object());
        }
    };
}

#endif

int run() {
    return 0;
}

