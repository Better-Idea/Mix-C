#ifndef xpack_lock_mutex
#define xpack_lock_mutex
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lock_mutex
        #include"define/base_type.hpp"
        #include"lock/atom_add.hpp"
        #include"lock/atom_fetch_or.hpp"
        #include"lock/atom_sub.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lock_mutex {
        enum class lock_result{
            success,
            fail,
        };

        /*
        primer_func 合适的可选取值
        对于 32bit的 primer_sum 最多支持 59369 个线程重入同一类函数
     -> 59369 59377 59387 59393 59399 59407 59417 59419 59441 59443 59447 59453 59467 59471 59473 59497 59509 59513 59539 59557 59561 59567 59581 59611 59617 59621 59627 59629 59651 59659 59663 59669 59671 59693 59699
        59707 59723 59729 59743 59747 59753 59771 59779 59791 59797 59809 59833 59863 59879 59887 59921 59929 59951 59957 59971 59981 59999 60013 60017 60029 60037 60041 60077 60083 60089 60091 60101 60103 60107 60127
        60133 60139 60149 60161 60167 60169 60209 60217 60223 60251 60257 60259 60271 60289 60293 60317 60331 60337 60343 60353 60373 60383 60397 60413 60427 60443 60449 60457 60493 60497
        */

        struct mutex {
            xgc_fields(
                xiam(mutex),
                xpro(primer_sum, volatile u32)
            )

            mutex() : primer_sum(0){}

            template<class lambda>
            lock_result try_lock(uxx primer_func, lambda const & invoke) {
                if (inc::atom_fetch_or(& primer_func, 1) == 0) {
                    primer_sum = primer_func;
                    invoke();
                    inc::atom_sub(& primer_sum, primer_func); // 用原子减法而不是直接置零
                    return lock_result::success;
                }
                else if (primer_sum % primer_func == 0){ // 能整除：指示是可以同时执行的行为
                    inc::atom_add(& primer_sum, primer_func);
                    invoke();
                    inc::atom_sub(& primer_sum, primer_func);
                    return lock_result::success;
                }
                else {
                    return lock_result::fail;
                }
            }
        };
    }
#endif

namespace xuser::inc {
    using ::mixc::lock_mutex::mutex;
}
