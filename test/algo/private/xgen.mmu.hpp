#ifdef xa_initial_length
using namespace inc;

auto alloc = [](uxx bytes){
    return inc::alloc<void>(inc::memory_size{bytes});
};

auto free = [](voidp ptr, uxx bytes){
    inc::free(ptr, inc::memory_size{bytes});
};

#ifdef xa_use_stack_table
#undef xa_use_stack_table
using var = var_array<xa_initial_length, with_fixed_page_table>;
item *  stack_table[32];
item ** table   = stack_table;
#else
using var       = var_array<xa_initial_length>;
item ** table   = nullptr;
#endif

uxx     length  = 0;
item    meta;
uxx     actual;
uxx     wanted;
uxx     cost = used_bytes();
meta.counter = 0;

for(uxx i = 0; i < xa_initial_length * 32; i++){
    for(uxx j = 0; j <= i; j++){
        meta.value = j;
        var::push(xref(table), xref(length), meta, alloc, free);
        xfail_ifne(j + 1, length);
    }
    for(uxx j = 0; j <= i; j++){
        auto & current = var::access(table, j);
        xfail_ifne(j, current.value);
    }

    var::clear(xref(table), xref(length), free);
    xfail_ifne(i + 1, meta.counter);
    xfail_ifne(cost, used_bytes());
    meta.counter = 0;
}

for(uxx i = 0; i < xa_initial_length * 32; i++){
    for(uxx j = 0; j <= i; j++){
        meta.value = j;
        var::push(xref(table), xref(length), meta, alloc, free);
    }
    for(uxx j = 0; j <= i; j++){
        auto & current = var::access(table, j);
        xfail_ifne(j, current.value);
    }

    auto destroy_count = i + 1;

    for(uxx j = 0, k = length / 2; j < k; j++){
        var::pop(xref(table), xref(length), xref(meta), alloc, free);
        xfail_ifne(1, meta.counter);
        xfail_ifne(length, meta.value);
        meta.counter    = 0;
        destroy_count  -= 1;
    }

    var::clear(xref(table), xref(length), free);
    xfail_ifne(destroy_count, meta.counter);
    xfail_ifne(cost, used_bytes());
    meta.counter = 0;
}

for(uxx i = 0; i < xa_initial_length * 32; i++){
    meta.value = i;
    var::push(xref(table), xref(length), meta, alloc, free);
}

while(length != 0){
    var::pop(xref(table), xref(length), xref(meta), alloc, free);
    xfail_ifne(length, meta.value);
    xfail_ifne(1, meta.counter);
    meta.counter = 0;
}
xfail_ifne(cost, used_bytes());

#undef xa_initial_length
#endif