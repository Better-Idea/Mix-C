#ifndef xpack_draft_x64
#define xpack_draft_x86
#include"configure/switch.hpp"
#include"define/base_type.hpp"

#if xis_msvc_native
    #include<intrin.h>
#else
    #include<x86intrin.h>
#endif

#define xgen(n)                             \
operator u64 & (){                          \
    return *u64p(this);                     \
}                                           \
void load(){                                \
    auto & mem = operator u64 &();          \
    asm("mov %%cr" #n ", %0":"=r"(mem));    \
}                                           \
void store(){                               \
    auto & mem = operator u64 &();          \
    asm("mov %0, %%cr" #n::"r"(mem));       \
}

namespace mixc::draft_x64{
    struct cr0_t{
        u64 protection_enabled      : 1;
        u64 monitor_coprocessor     : 1;
        u64 emulation               : 1;
        u64 task_switched           : 1;
        u64 extension_type          : 1;
        u64 numeric_error           : 1;
        u64                         : 10;

        u64 write_protect           : 1;
        u64                         : 1;
        u64 alignment_mask          : 1;
        u64                         : 10;
        u64 not_writethrough        : 1;
        u64 cache_disable           : 1;
        u64 paging                  : 1;
        u64                         : 32;

        xgen(0)
    };

    struct cr2_t{
        operator u64(){
            u64 mem;
            asm("mov %%cr2, %0":"=r"(mem));
            return mem;
        }
    };

    struct cr3_t{
        u64 process_context_identifier              : 12; // enable it in 
        u64 lv4_page_table                          : 40;

        xgen(3)
    };

    struct cr4_t{
        u64 virtual8086_mode_extensions             : 1;
        u64 protected_mode_virtual_interrupts       : 1;
        u64 time_stamp_disable                      : 1;
        u64 debugging_extensions                    : 1;
        u64 page_size_extensions                    : 1;
        u64 physical_address_extension              : 1;
        u64 machine_check_enable                    : 1;
        u64 page_global_enable                      : 1;
        u64 performance_monitoring_counter_enable   : 1;
        u64 os_fxsave_fxrstor_support               : 1;
        u64 os_unmasked_exception_support           : 1;
        u64 user_mode_instruction_prevention        : 1;
        u64                                         : 4;
        u64 rwbase_instructions_enable              : 1;
        u64 process_context_identifier_enable       : 1;
        u64 osxsave_xsave_extended_states_enable    : 1;
        u64                                         : 1;
        u64 supervisor_mode_execution_prevention    : 1;
        u64 supervisor_mode_access_protection       : 1;
        u64 protection_key_enable                   : 1;

        xgen(4)
    };

    #undef  xgen

    struct rflags_t{
        u64 carry_flag                              : 1;
        u64                                         : 1;
        u64 parity_flag                             : 1;
        u64                                         : 1;
        u64 auxiliary_flag                          : 1;
        u64                                         : 1;
        u64 zero_flag                               : 1;
        u64 sign_flag                               : 1;
        u64 trap_flag                               : 1;
        u64 interrupt_flag                          : 1;
        u64 direction_flag                          : 1;
        u64 overflow_flag                           : 1;
        u64 io_privilege_level                      : 2;
        u64 nested_task                             : 1;
        u64                                         : 1;
        u64 resume_flag                             : 1;
        u64 virtual8086_mode                        : 1;
        u64 alignment_check                         : 1;
        u64 virtual_interrupt_flag                  : 1;
        u64 virtual_interrupt_pending               : 1;
        u64 id_flag                                 : 1;
    };

    struct efer_t{
        u64 system_call_extensions                  : 1;
        u64                                         : 7;
        u64 long_mode_enable                        : 1;
        u64                                         : 1;
        u64 long_mode_active                        : 1;
        u64 no_execute_enable                       : 1;
        u64 secure_virtual_machine_enable           : 1;
        u64 long_mode_segment_limit_enable          : 1;
        u64 fast_fxsave_fxrstor                     : 1;
        u64 translation_cache_extension             : 1;

        operator u64 & (){
            return *u64p(this);
        }

        void load(){
            operator u64 &() = *u64p(0xc0000080);
        }

        void store(){
            *u64p(0xc0000080) = this[0];
        }
    };

    struct code_segment_descriptor{
        u64                                         : 32 + 8 + 2;
        u64 confirm                                 : 1;
    private:
        u64 type                                    : 2;
    public:
        u64 dpl                                     : 2;
        u64 present                                 : 1;
        u64                                         : 5;
        u64 long_attribute_bit                      : 1;
        u64 default_operrand_size                   : 1;

        code_segment_descriptor(){
            operator u64 &()    = 0;
            type                = 0x3; // for code segment
        }

        operator u64 & (){
            return *u64p(this);
        }
    };

    struct data_segment_descriptor{
        u64                                         : 32 + 8 + 2 + 1;
    private:
        u64 type                                    : 2;
    public:
        u64                                         : 2;
        u64 present                                 : 1;
        u64                                         : 5;

        data_segment_descriptor(){
            operator u64 &()    = 0;
            type                = 0x2; // for data segment
        }

        operator u64 & (){
            return *u64p(this);
        }
    };

    enum class sysseg_t{
        ldt             = 0x2,
        tss             = 0x9,
        tss_busy        = 0xb,
        call_gate       = 0xc,
        interrupt_gate  = 0xe,
        trap_gate       = 0xf,
    };

    struct system_segment_descriptor{
        system_segment_descriptor(){
            (u64p(this))[0] = 0;
            (u64p(this))[1] = 0;
        }
        auto & segment_limit(uxx value){
            segment_limit_low16 = value;
            segment_limit_high4 = value >> 16;
            return this[0];
        }
        auto & type(sysseg_t value){
            ptype = uxx(value);
            return this[0];
        }
        auto & base_address(u64 value){
            base_address_low24      = value;
            base_address_middle8    = value >> 24;
            base_address_high32     = value >> 32;
            return this[0];
        }
        auto & present(bool value){
            ppresent = value;
            return this[0];
        }
        auto & big_granularity(bool value){
            pbig_granularity = value;
            return this[0];
        }
        auto & dpl(uxx value){
            pdpl = value;
            return this[0];
        }
    private:
        u64 segment_limit_low16                     : 16;
        u64 base_address_low24                      : 24;

        u64 ptype                                   : 4;
        u64                                         : 1;
        u64 pdpl                                    : 2;
        u64 ppresent                                : 1;

        u64 segment_limit_high4                     : 4;
        u64 available_bit                           : 1;
        u64                                         : 2;
        u64 pbig_granularity                        : 1;
        u64 base_address_middle8                    : 8;
        u64 base_address_high32                     : 32;
    };

    struct call_gate_descriptor{
        call_gate_descriptor(){
            (u64p(this))[0] = 0;
            (u64p(this))[1] = 0;
            ptype = uxx(sysseg_t::call_gate);
        }
        auto & target_selector(uxx value){
            ptarget_selector = value;
            return this[0];
        }
        auto & target_offset(u64 value){
            target_offset_low16     = value;
            target_offset_middle16  = value >> 16;
            target_offset_high32    = value >> 32;
            return this[0];
        }
        auto & present(bool value){
            ppresent = value;
            return this[0];
        }
        auto & dpl(uxx value){
            pdpl = value;
            return this[0];
        }
    protected:
        u64 target_offset_low16                     : 16;
        u64 ptarget_selector                        : 16;
        u64 ist                                     : 3;
        u64                                         : 5;
        u64 ptype                                   : 4;
        u64                                         : 1;
        u64 pdpl                                    : 2;
        u64 ppresent                                : 1;
        u64 target_offset_middle16                  : 16;
        u64 target_offset_high32                    : 32;
    };

    struct interrupt_gate_descriptor : call_gate_descriptor{
        interrupt_gate_descriptor(){
            ptype = uxx(sysseg_t::interrupt_gate);
        }
    };

    struct page_table{
        auto & present(bool value){
            ppresent = value;
            return this[0];
        }
        auto & read_write(bool value){
            pread_write = value;
            return this[0];
        }
        auto & user(bool value){
            puser = value;
            return this[0];
        }
        auto & page_write_through(bool value){
            ppage_write_through = value;
            return this[0];
        }
        auto & page_cache_disable(bool value){
            ppage_cache_disable = value;
            return this[0];
        }
        auto & accessed(bool value){
            paccessed = value;
            return this[0];
        }
        bool accessed(){
            return paccessed;
        }
        auto & has_next_level_page(bool value){
            phas_next_level_page = value;
            return this[0];
        }
        auto & available(u64 value){
            pavailable_low3     = value;
            pavailable_high11   = value >> 3;
            return this[0];
        }
        auto & table_address(u64 value){
            ptable_address = value >> 12;
            return this[0];
        }
        auto table_address(){
            return u64(ptable_address) << 12;
        }
        auto & not_executable(bool value){
            pnot_executable = value;
            return this[0];
        }
    protected:
        u64 ppresent                                : 1;
        u64 pread_write                             : 1;    // 当 cr0.write_protect = 1 时，最高权限也不能修改只读段(否则是可以修改的)
        u64 puser                                   : 1;    // 允许系统访问用户内存
        u64 ppage_write_through                     : 1;
        u64 ppage_cache_disable                     : 1;
        u64 paccessed                               : 1;
        u64 pdirty                                  : 1;
        u64 phas_next_level_page                    : 1;
        u64                                         : 1;
        u64 pavailable_low3                         : 3;
        u64 ptable_address                          : 40;
        u64 pavailable_high11                       : 11;
        u64 pnot_executable                         : 1;    // 需要设置 efer.nxe = 1 才能使用该位
    };

    #pragma pack(2)
    struct descriptor_table{
        u16 limit;
        u64 base;
    };
    #pragma pack()

    #define xgen(name)                                      \
    struct name ## _t : segment_register{                   \
        void load(){                                        \
            asm("mov %%" # name ", %0":"=r"(*(u16 *)this)); \
        }                                                   \
        void store(){                                       \
            asm("mov %0, %%" # name::"r"(*(u16 *)this));    \
        }                                                   \
    };

    struct segment_register{
        u16 cpl                                     : 2;
        u16 in_ldt                                  : 1;
        u16 index                                   : 13;
    };

    xgen(cs)
    xgen(ds)
    xgen(es)
    xgen(fs)
    xgen(gs)
    xgen(ss)

    #undef xgen

    #define xstore_flags()  __write_flags(*(uxx *)& flags)
    #define xload_flags()   *((uxx *)& flags) = __read_flags()

    enum class port_t : u16{
        m8259_data          = 0x20,
        m8259_cmd           = 0x21,
        keyboard_data       = 0x60,
        keyboard_status     = 0x64,
        keyboard_cmd        = 0x64,
        s8259_data          = 0xa0,
        s8259_cmd           = 0xa1,
    };

    inline void out(port_t port, u08 value){
        asm("outb %0, %1"::"a"(value), "d"((u16)port));
    }

    inline void outw(port_t port, u16 value) {
        asm("outw %0, %1"::"a"(value), "d"((u16)port));
    }

    inline void outd(port_t port, u32 value) {
        asm("outl %0, %1"::"a"(value), "d"((u32)port));
    }

    inline u08 inb(port_t port) {
        u08 value;
        asm("inb %1, %0":"=a"(value) : "d"((u16)port));
        return value;
    }
    inline u16 inw(port_t port) {
        u16 value;
        asm("inw %1, %0":"=a"(value) : "d"((u16)port));
        return value;
    }
    inline u32 ind(port_t port) {
        u32 value;
        asm("inl %1, %0":"=a"(value):"d"((u16)port));
        return value;
    }

    static inline cr0_t     cr0;
    static inline cr2_t     page_fault_virtual_address;
    static inline cr3_t     cr3;
    static inline cr4_t     cr4;
    static inline efer_t    efer;
    static inline rflags_t  rflags;
    static inline cs_t      cs;
    static inline ds_t      ds;
    static inline es_t      es;
    static inline fs_t      fs;
    static inline gs_t      gs;
    static inline ss_t      ss;
}

#endif

namespace xuser{
    using namespace ::mixc::draft_x64;
}
