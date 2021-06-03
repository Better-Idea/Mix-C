#pragma once

namespace mixc::macro_xref{
    template<class type_t> struct meta{
        using type = type_t;
    };

    template<class type_t> struct meta<type_t &>{
        using type = type_t;
    };

    template<class type_t> struct meta<type_t const &>{
        using type = type_t;
    };

    template<class type_t>
    using remove_ref = typename meta<type_t>::type;
}

#define xref(...)                                                                               \
    (                                                                                           \
        (::mixc::macro_xref::remove_ref<decltype(__VA_ARGS__)> *)                               \
        (& (void* &)(::mixc::macro_xref::remove_ref<decltype(__VA_ARGS__)> &)(__VA_ARGS__))     \
    )
