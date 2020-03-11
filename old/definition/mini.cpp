#include"component/memory.hpp"
#include"definition/mini.hpp"
#define inline

i32 global_miss_test_count;
i32 global_test_error_count;

xspace()
    volatile void *  null = nullptr;
    volatile void ** volatile_null = & null;
$

xspace(mixc) xspace(component)
    uxx alloc_indicator;
    uxx alloc_bytes;
    uxx actually_alloc_bytes;
    memory_private::allocator mem;

    xdefr(uxx, memory::allocated_bytes)
        return alloc_bytes;
    $

    xdefr(uxx, memory::survival_of_memory)
        return alloc_indicator;
    $

    xdefr(uxx, memory::actually_alloc_bytes)
        return actually_alloc_bytes;
    $

    xdefr(voidp, memory::origin_alloc, uxx bytes)
        alloc_indicator++;
        alloc_bytes += bytes;
        return mem.alloc(bytes);
    $

    xdefr(void, memory::origin_free, voidp ptr, uxx bytes)
        alloc_indicator--;
        alloc_bytes -= bytes;
        mem.free(ptr, bytes);
    $
$ $

//一些编译器不允许nullptr引用,所以设置成volatile初始化
voidp nullref_t::value = (voidp)volatile_null[0];


