#pragma once

#define xis_nullptr(...)                                                        \
bool operator==(decltype(nullptr)) const {                                      \
    return __VA_ARGS__;                                                         \
}                                                                               \
friend bool operator==(decltype(nullptr), the_t const & self){                  \
    return self == nullptr;                                                     \
}                                                                               \
friend bool operator!=(the_t const & self, decltype(nullptr)){                  \
    return !(self == nullptr);                                                  \
}                                                                               \
friend bool operator!=(decltype(nullptr), the_t const & self){                  \
    return !(self == nullptr);                                                  \
}
