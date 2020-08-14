#ifdef xuser
    #undef xuser
#endif
#include"hpc/mpc.hpp"
#include<omp.h>

namespace mixc::phc_mpc{
    static_assert(sizeof(core_lock_t) == sizeof(omp_lock_t));

    void core_lock_init(voidp item){
        omp_init_lock((omp_lock_t *)item);
    }

    void core_lock_set(voidp item){
        omp_set_lock((omp_lock_t *)item);
    }

    void core_lock_unset(voidp item){
        omp_unset_lock((omp_lock_t *)item);
    }

    void core_lock_deinit(voidp item){
        omp_destroy_lock((omp_lock_t *)item);
    }

    uxx mpc_t::logic_cores(){
        return omp_get_num_procs();
    }

    uxx mpc_t::current_core(){
        return omp_get_thread_num();
    }

    uxx mpc_t::concurrency(){
        return omp_get_num_threads();
    }

    void mpc_t::concurrency(uxx value){
        omp_set_num_threads(value);
    }
}
