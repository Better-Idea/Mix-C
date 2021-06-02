#pragma once
#define xref(...)       ((decltype(__VA_ARGS__) *) (& (void* &)(decltype(__VA_ARGS__) &)(__VA_ARGS__)))
