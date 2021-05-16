#pragma once
#define xcheck_binary_aligned(value_v) \
        static_assert(!((value_v) & ((value_v) - 1)), #value_v "need binary aligned");