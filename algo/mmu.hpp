#ifndef xpack_algo_mmu
#define xpack_algo_mmu
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::algo_mmu
        #include"define/base_type.hpp"
        #include"instruction/index_of_last_set.hpp"
        #include"instruction/bit_test_and_reset.hpp"
        #include"memory/allocator.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::algo_mmu {
        struct pagoff{ uxx page, offset; };

        struct addressing{
            static auto var(uxx present_indicator, uxx address){
                uxx    mask = present_indicator & address;
                pagoff r { .page = inc::index_of_last_set(mask) + 1, .offset = address };

                if (r.page != 0){
                    inc::bit_test_and_reset(& r.offset, r.page);
                }
                return r;
            }

            static auto fixed(uxx present_indicator, uxx address){
                pagoff r;
                uxx mask = present_indicator ^ address;
                r.page   = inc::index_of_last_set(mask);
                r.offset = address;
                return r;
            }
        };

        template<uxx small_page_count, uxx step_exp = 8>
        union page_var{
            using the_t = page_var<small_page_count, step_exp>;
        //     static auto alloc(uxx capacity){
        //         return inc::alloc_with_initial<the_t>(
        //             inc::memory_size(
        //                 size(capacity)
        //             ),
        //             capacity
        //         );
        //     }
        // protected:
        //     page_var(uxx capcity) : 
        //         capcity(capcity), bytes(0){}

            static uxx size(uxx capacity){
                uxx     i     = (inc::index_of_last_set(capacity) + 1);
                uxx     mask  = (uxx(1) << small_page_count) - 1;
                uxx     spc   = (i >> (step_exp)) & mask;
                uxx     bpc   = (i >> (step_exp + small_page_count));
                uxx     bytes = (bpc + spc + 2) * sizeof(uxx);
                return  bytes;
            }

            template<class type>
            type & access(uxx address){
                using typep = type *;
                auto r = addressing::var(bytes, address);

                if (r.page < step_exp){
                    return typep(spage[0] + r.offset)[0];
                }
                if (r.page -= step_exp; r.page < small_page_count){
                    return typep(spage[r.page] + r.offset)[0];
                }

                r.page    -= small_page_count;
                r.offset  &= bpage_mask;
                return typep(bpage[r.page] + r.offset)[0];
            }

            uxx    bytes;
            uxx    capcity;
            u08 *  spage[small_page_count];
            u08 *  bpage[1];

            static constexpr uxx bpage_size = uxx(1) << (step_exp + small_page_count);
            static constexpr uxx bpage_mask = bpage_size - 1;

            #define xgen(name,type)                                                 \
            struct{                                                                 \
                type & operator [](uxx address){                                    \
                    return the.template access<type>(address * sizeof(type));       \
                }                                                                   \
            } name
        public:
            xgen(mu08, u08);
            xgen(mu16, u16);
            xgen(mu32, u32);
            xgen(mu64, u64);
            xgen(mi08, i08);
            xgen(mi16, i16);
            xgen(mi32, i32);
            xgen(mi64, i64);
            xgen(mf32, f32);
            xgen(mf64, f64);
            #undef xgen
        };

    }

#endif

namespace xuser::inc{
    using ::mixc::algo_mmu::addressing;
    using ::mixc::algo_mmu::page_var;
}
