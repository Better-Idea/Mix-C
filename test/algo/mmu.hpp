#ifndef xpack_test_algo_mmu
#define xpack_test_algo_mmu
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::test_algo_mmu::inc
#include"algo/mmu.hpp"
#include"macro/xassert.hpp"
#include"memory/allocator.hpp"
#pragma pop_macro("xuser")

namespace mixc::test_algo_mmu{
    struct item{
        static inline uxx counter = 0;

        uxx value;

        ~item(){
            counter += 1;
        }
    };

    xtest("mmu_var_array_1") {
        #define xa_initial_length   1
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_2") {
        #define xa_initial_length   2
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_4") {
        #define xa_initial_length   4
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_8") {
        #define xa_initial_length   8
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_16") {
        #define xa_initial_length   16
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_32") {
        #define xa_initial_length   32
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_64") {
        #define xa_initial_length   64
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_1s") {
        #define xa_initial_length   1
        #define xa_use_stack_table
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_2s") {
        #define xa_initial_length   2
        #define xa_use_stack_table
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_4s") {
        #define xa_initial_length   4
        #define xa_use_stack_table
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_8s") {
        #define xa_initial_length   8
        #define xa_use_stack_table
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_16s") {
        #define xa_initial_length 16
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_32s") {
        #define xa_initial_length 32
        #include"test/algo/private/xgen.mmu.hpp"
    };

    xtest("mmu_var_array_64s") {
        #define xa_initial_length 64
        #include"test/algo/private/xgen.mmu.hpp"
    };
}

#endif
