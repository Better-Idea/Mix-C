#ifdef xinitial_length
using namespace inc;

auto alloc = [](uxx bytes){
    return inc::alloc<void>(inc::memory_size{bytes});
};

auto free = [](voidp ptr, uxx bytes){
    inc::free(ptr, inc::memory_size{bytes});
};

item ** table   = nullptr;
uxx     length  = 0;
item    meta;
uxx     actual;
uxx     wanted;
uxx     cost = used_bytes();
meta.counter = 0;

for(uxx i = 0; i < xinitial_length * 32; i++){
    for(uxx j = 0; j <= i; j++){
        meta.value = j;
        var_array<xinitial_length>::push(xref table, xref length, meta, alloc, free);
        xassert_eq(j + 1, length);
    }
    for(uxx j = 0; j <= i; j++){
        auto & current = var_array<xinitial_length>::access(table, length, j);
        xassert_eq(j, current.value);
    }

    var_array<xinitial_length>::clear(xref table, xref length, free);
    xassert_eq(i + 1, meta.counter);
    xassert_eq(cost, used_bytes());
    meta.counter = 0;
}

for(uxx i = 0; i < xinitial_length * 32; i++){
    for(uxx j = 0; j <= i; j++){
        meta.value = j;
        var_array<xinitial_length>::push(xref table, xref length, meta, alloc, free);
    }
    for(uxx j = 0; j <= i; j++){
        auto & current = var_array<xinitial_length>::access(table, length, j);
        xassert_eq(j, current.value);
    }

    auto destroy_count = i + 1;

    for(uxx j = 0, k = length / 2; j < k; j++){
        var_array<xinitial_length>::pop(xref table, xref length, xref meta, alloc, free);
        xassert_eq(1, meta.counter);
        xassert_eq(length, meta.value);
        meta.counter    = 0;
        destroy_count  -= 1;
    }

    inc::var_array<xinitial_length>::clear(xref table, xref length, free);
    xassert_eq(destroy_count, meta.counter);
    xassert_eq(cost, used_bytes());
    meta.counter = 0;
}

for(uxx i = 0; i < xinitial_length * 32; i++){
    meta.value = i;
    var_array<xinitial_length>::push(xref table, xref length, meta, alloc, free);
}

while(length != 0){
    var_array<xinitial_length>::pop(xref table, xref length, xref meta, alloc, free);
    xassert_eq(length, meta.value);
    xassert_eq(1, meta.counter);
    meta.counter = 0;
}
xassert_eq(cost, used_bytes());

#undef xinitial_length
#endif