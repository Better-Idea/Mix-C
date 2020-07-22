#pragma once
#define xfinal_tmpl(pack,name,...)                              \
struct name : pack::name<name<__VA_ARGS__>, __VA_ARGS__> {      \
    using pack::name<name<__VA_ARGS__>, __VA_ARGS__>::name;     \
}

#define xfinal(pack,name)                                       \
struct name : pack::name<name> {                                \
    using pack::name<name>::name;                               \
}
