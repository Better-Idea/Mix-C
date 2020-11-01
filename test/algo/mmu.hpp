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

    xtest(mmu_var_array_1) {
        #define xinitial_length 1
        #include"test/algo/template/mmu.hpp"
    };

    xtest(mmu_var_array_2) {
        #define xinitial_length 2
        #include"test/algo/template/mmu.hpp"
    };

    xtest(mmu_var_array_4) {
        #define xinitial_length 4
        #include"test/algo/template/mmu.hpp"
    };

    xtest(mmu_var_array_8) {
        #define xinitial_length 8
        #include"test/algo/template/mmu.hpp"
    };

    xtest(mmu_var_array_16) {
        #define xinitial_length 16
        #include"test/algo/template/mmu.hpp"
    };

    xtest(mmu_var_array_32) {
        #define xinitial_length 32
        #include"test/algo/template/mmu.hpp"
    };

    xtest(mmu_var_array_64) {
        #define xinitial_length 64
        #include"test/algo/template/mmu.hpp"
    };
}

#endif
