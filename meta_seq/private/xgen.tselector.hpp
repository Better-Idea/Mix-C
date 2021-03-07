#pragma once
#define xmake_selector(the_item)                \
template<class tuple_t>                         \
struct tselector_ ## the_item {                 \
    using item_t = typename tuple_t::the_item;  \
}
