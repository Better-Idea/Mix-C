#pragma once

#define xis_nullptr(...)                                                        \
bool operator==(decltype(nullptr)) const {                                      \
    return __VA_ARGS__;                                                         \
}                                                                               \
friend bool operator==(decltype(nullptr), __self const & self){                 \
    return self == nullptr;                                                     \
}                                                                               \
friend bool operator!=(__self const & self, decltype(nullptr)){                 \
    return !(self == nullptr);                                                  \
}                                                                               \
friend bool operator!=(decltype(nullptr), __self const & self){                 \
    return !(self == nullptr);                                                  \
}
