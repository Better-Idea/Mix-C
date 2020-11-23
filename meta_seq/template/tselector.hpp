#pragma once
#define xmake_selector(the_item)                \
template<class tuple>                           \
struct tselector_ ## the_item {                 \
    using item_t = typename tuple::the_item;      \
}
