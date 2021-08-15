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
#include"utils/memory.hpp"
#include"macro/xassert.hpp"
#include"math/random.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::test_utils_allocator{
    constexpr uxx count = 1024 * 1024;
    inline uxxp record[count];
    inline uxx thread_count = 0;
    inline uxx times = 0;

    xtest("memory-bin"){
        uxx flow_count = 1;
        xhint(inc::memory::alive_pages());

        {
            inc::thread tx = xjoinable{
                for(uxx i = 0; i < count; i++){
                    record[i] = inc::memory::alloc<uxx>();
                    record[i][0] = 0;
                    inc::atom_sub(xref(times), 1);
                }

                uxx i = 0;
            };
        }

        for(uxx i = 0; i < count; i++) {
            inc::memory::free<uxx>(record[i]);
        }

        while (inc::memory::alive_pages() > 2){
            inc::thread_self::sleep(1000);
            xhint(inc::memory::alive_pages());
        }
    };

    xtest("memory-walk"){
        uxx flow_count = 1;

        xhint(inc::memory::alive_pages());
        inc::atom_add(xref(thread_count), flow_count);

        {
            inc::thread trace = xjoinable{
                while(inc::atom_load(xref(thread_count))){
                    inc::thread_self::sleep(1000);
                    xhint(inc::memory::alive_pages());
                }
            };

             for(uxx i = 0; i < flow_count; i++){
                // 并发创建多个线程
                inc::thread tx = xdetached{
                    for(uxx i = 0; i < count;){
                        auto & r = record[i];
                        auto   alloc = inc::random<bool>();
                        auto   bytes = inc::random<uxx>() % 128 + 1;
                        auto   size = inc::memory::size{ bytes };
                        auto   ptr = alloc ? inc::memory::alloc_with_initial<uxx>(size, bytes) : uxxp(nullptr);
                        auto   mem = inc::atom_swap(xref(r), ptr); 

                        if (mem){
                            inc::memory::free(mem, inc::memory::size{  *mem });
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
                inc::memory::free(mem, inc::memory::size{ *mem });
            }
        }

        while (inc::memory::alive_pages() > 2) {
            inc::thread_self::sleep(1000);
            xhint(inc::memory::alive_pages());
        }
    };
}

#endif

