#ifndef xpack_extern_rtos_sync
#define xpack_extern_rtos_sync
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::extern_rtos_sync::inc
#include"define/base_type.hpp"
#include"dumb/disable_copy.hpp"
#include"extern/rtos/conf.hpp"
#include"extern/rtos/task.hpp"
#include"instruction/bit_test_and_set.hpp"
#include"meta/fit_bits.hpp"
#pragma pop_macro("xuser")

namespace mixc::extern_rtos_sync::origin{
    // TODO：适应更多优先级的情况
    using bmp_t = inc::fit_bits<inc::conf::max_priority>;

    struct sync : inc::disable_copy{
    private:
        using tcp_t = inc::task_context *;

        // TODO：优先级较少时，只有一层
        tcp_t **    blocked_table   = nullptr;
        bmp_t       bmp             = 0;
    public:
        void wait(){
            tcp_t    table_lv1[inc::conf::length_of_blocked_line];
            tcp_t  * table_lv0[inc::conf::length_of_blocked_page];

            inc::g_task_self.critical([&](){
                auto self                   = inc::g_task_self.m_handle;
                auto i                      = inc::g_task_self.priority_current();
                auto i0                     = i / inc::conf::length_of_blocked_line;
                auto i1                     = i % inc::conf::length_of_blocked_line;

                if (this->bmp == 0){
                    this->blocked_table     = table_lv0;
                }

                if (inc::bit_test_and_set(xref this->bmp, i)){
                    auto page               = this->blocked_table[i0];
                    auto head               = page[i1];
                    auto prev               = head->prev;
                    prev->next              = self;
                    head->prev              = self;
                    self->next              = head;
                    self->prev              = prev;
                }
                else{
                    auto page               = this->blocked_table[i0] = table_lv1;
                    self->next              = self;
                    self->prev              = self;
                    page[i1]                = self;
                }
            });

            inc::g_task_list.switch_to_first_ready();
        }

        void signal(){

        }
    };
}

#endif

