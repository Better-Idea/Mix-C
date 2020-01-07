#include"component/memory.hpp"
#include"definition/mini.hpp"
#define inline

i32 global_miss_test_count;
i32 global_test_error_count;

xspace()
    volatile void *  null = nullptr;
    volatile void ** volatile_null = & null;
$

xspace(mixc)
    xspace(memory_private)
        uxx alloc_indicator;
        uxx alloc_bytes;
        uxx actually_alloc_bytes;
        allocator mem;
    $
$

//一些编译器不允许nullptr引用,所以设置成volatile初始化
voidp nullref_t::value = (voidp)volatile_null[0];
xusing mixc;

xdefr(uxx, memory::allocated_bytes)
    return memory_private::alloc_bytes;
$

xdefr(uxx, memory::survival_of_memory)
    return memory_private::alloc_indicator;
$

xdefr(uxx, memory::actually_alloc_bytes)
    return memory_private::actually_alloc_bytes;
$

xdefr(voidp, memory::origin_alloc, uxx bytes)
    memory_private::alloc_indicator++;
    memory_private::alloc_bytes += bytes;
    return memory_private::mem.alloc(bytes);
$

xdefr(void, memory::origin_free, voidp ptr, uxx bytes)
    memory_private::alloc_indicator--;
    memory_private::alloc_bytes -= bytes;
    memory_private::mem.free(ptr, bytes);
$

