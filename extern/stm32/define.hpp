#pragma once
#pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc
    #include"define/base_type.hpp"
    #include"macro/xlink.hpp"
    #include"macro/xvolatile.hpp"
    #include"instruction/index_of_first_set.hpp"
#pragma pop_macro("xuser")

#define xfield(name,bits)               name, xlink(end_, name) = name + bits - 1
#define xrsv(bits)                      xfield(xlink2(rsv, __COUNTER__),bits)

#define xrox(type,name,...)                                                         \
type name() {                                                                       \
    return type(__VA_ARGS__);                                                       \
}                                                                                   \
auto & name(type * get) {                                                           \
    get[0] = name();                                                                \
    return this[0];                                                                 \
}                                   

#define xwox(type,name,...)                                                         \
auto & name(type value) {                                                           \
    __VA_ARGS__;                                                                    \
    return this[0];                                                                 \
}

#define xinner(peripheral)                                                          \
    using the_t = peripheral<mmio_address>;                                         \
    struct inner{                                                                   \
        enum{                                                                       \
            field_length        = field_bits / 32,                                  \
            indicator_length    = field_length / 32 + (field_length % 32 != 0),     \
        };                                                                          \
        static inline u32 field[field_length];                                      \
        static inline u32 indicator[indicator_length];                              \
    };

#define xset(name,value)                mcu_bop  (mmio_address, inner::field, inner::indicator, name, xlink(end_, name), u32(value))
#define xget8(name)                     mcu_bop8 (mmio_address, name)
#define xset8(name,value)               mcu_bop8 (mmio_address, name, u32(value))
#define xget16(name)                    mcu_bop16(mmio_address, name)
#define xset16(name,value)              mcu_bop16(mmio_address, name, u32(value))
#define xget(name)                      mcu_bop(mmio_address, name, xlink(end_, name))
#define xr1(type,name,field)            xrox(type, name, xget(field)) xwox(clear_t, name, xset(field, 1))
#define xro(type,name,field)            xrox(type, name, xget(field))
#define xwo(type,name,field)            xwox(type, name, xset(field, value))
#define xrw(type,name,field)            xro (type, name, field) xwo(type, name, field)
#define xrwx(type,name,read,write)      xrox(type, name, read) xwox(type,name, write)
#define xassume(type,value)             operator type () const { return type(value); }
#define xmmio(name)                     template<uxx mmio_address> struct name : base_peripheral<name, mmio_address>

namespace mixc{
    inline u32 mcu_bop(u32 mmio, u32 start_bits, u32 end_bits){
        asm volatile("":::"memory");
        auto   idx = start_bits >> 5;
        auto   ofs = start_bits & 0x1f;
        auto   len = end_bits - start_bits + 1;
        auto   mem = u32p(mmio)[idx];
        auto   msk = (1 << len) - 1;
        auto   val = (mem >> ofs);
        return val & msk;
    }

    inline u32 mcu_bop8(u32 mmio, u32 start_bits){
        asm volatile("":::"memory");
        auto   idx = start_bits >> 3;
        return u08p(mmio)[idx];
    }

    inline u32 mcu_bop16(u32 mmio, u32 start_bits){
        asm volatile("":::"memory");
        auto   idx = start_bits >> 3;
        return u16p(u08p(mmio) + idx)[0];
    }

    inline u32 mcu_bop32(u32 mmio, u32 start_bits){
        asm volatile("":::"memory");
        auto   idx = start_bits >> 3;
        return u32p(u08p(mmio) + idx)[0];
    }

    inline void mcu_bop8(u32 mmio, u32 start_bits, u32 value){
        asm volatile("":::"memory");
        auto   idx = start_bits >> 3;
        (u08p(mmio))[idx] = value;
    }

    inline void mcu_bop16(u32 mmio, u32 start_bits, u32 value){
        asm volatile("":::"memory");
        auto   idx = start_bits >> 3;
        (u16p(u08p(mmio) + idx))[0] = value;
    }

    inline void mcu_bop32(u32 mmio, u32 start_bits, u32 value){
        asm volatile("":::"memory");
        auto   idx = start_bits >> 3;
        (u32p(u08p(mmio) + idx))[0] = value;
    }

    inline void mcu_bop(u32 mmio, u32p buffer, u32p record, u32 start_bits, u32 end_bits, u32 value){
        auto   idx = start_bits >> 5;
        auto   ofs = start_bits & 0x1f;
        auto   len = end_bits - start_bits + 1;
        auto & des = (buffer[idx]);
        auto & rcd = (record[idx >> 5]);
        auto   msk = ((1 << len) - 1) << ofs;
        auto   bit = (1 << (idx & 0x1f));
        auto   val = rcd & bit ? des : u32p(mmio)[idx];
        des        = (val & ~msk) | (value << ofs);
        rcd       |= bit;
    }

    inline void mcu_bop_direct(u32 mmio, u32 start_bits, u32 end_bits, u32 value){
        auto   idx = start_bits >> 5;
        auto   ofs = start_bits & 0x1f;
        auto   len = end_bits - start_bits + 1;
        auto   msk = ((1 << len) - 1) << ofs;
        auto & val = u32p(mmio)[idx];
        val        = (val & ~msk) | (value << ofs);
    }

    template<template<uxx> class peripheral, uxx mmio_address>
    struct base_peripheral{
        using the_t = peripheral<mmio_address>;

        static void flush(){
            using rfl = typename the_t::inner;
            u32  mmio           = mmio_address;
            u32  record_length  = rfl::indicator_length;
            u32p record         = rfl::indicator;
            u32p buffer         = rfl::field;

            for(u32 i = 0, * des = u32p(mmio); i < record_length; i++){
                while(true){
                    if (uxx t = inc::index_of_first_set(record[i]), 
                        idx = i << 5 | t; 
                        t == not_exist){
                        break;
                    }
                    else{
                        record[i] ^= 1 << t;
                        des[idx]   = buffer[idx];
                    }
                }
            }

            asm volatile("":::"memory");
        }
    };

    struct clear_t{
        constexpr clear_t(){}
    } constexpr clear{};

    enum class pwr_source_regulator{
        from_main_voltage_regulator,
        from_low_power_voltage_regulator,
    };

    enum class pwr_deep_sleep{
        enter_stop_mode,
        enter_standby,
    };

    enum class pwr_thresholds_voltage{
        at_2v0,
        at_2v1,
        at_2v3, // no 2v4
        at_2v5,
        at_2v6,
        at_2v7,
        at_2v8,
        at_2v9,
    };

    enum class pwr_backup_domain{
        enable_write_protection,
        disable_write_protection,
    };

    enum class pwr_flash{
        keep_power,
        power_down,
    };

    enum class pwr_regulator{
        keep_enable,
        enter_under_drive_mode,
    };

    enum class pwr_regulator_scaling{
        lv3 = 1, // reverse order
        lv2,
        lv1
    };

    enum class pwr_wu_polarity{
        rising_edge,
        falling_edge,
    };

    enum class rcc_low_power_mode{
        close_clock_when_sleep,
        keep_clock,
    };

    enum class rcc_for_rtc{
        none,
        div2 = 2, div3, div4, div5, div6, div7, div8, div9, div10, div11, div12, div13, div14, div15, div16, div17, div18, div19, div20, div21, div22, div23, div24, div25, div26, div27, div28, div29, div30, div31,
    };

    enum class rcc_for_mco{
        none,
        div2 = 4, div3, div4, div5,
    };

    enum class rcc_for_sclk{
        div2, div4, div6, div8,
    };

    enum class rcc_for_otg_sdio_random{
        div2 = 2, div3, div4, div5, div6, div7, div8, div9, div10, div11, div12, div13, div14, div15,
    };

    enum class rcc_source_for_pll{
        from_hsi,
        from_hse,
    };

    enum class rcc_source_for_sclk{
        from_hsi,
        from_hse,
        from_pll,
    };

    enum class rcc_source_for_ahb{
        from_sclk,
        from_sclk_div2 = 8,
        from_sclk_div4,
        from_sclk_div8,
        from_sclk_div16,
        from_sclk_div64,
        from_sclk_div128,
        from_sclk_div256,
        from_sclk_div512,
    };

    enum class rcc_source_for_apb{
        from_ahb,
        from_ahb_div2 = 4,
        from_ahb_div4,
        from_ahb_div8,
        from_ahb_div16,
    };

    enum class rcc_source_for_i2s{
        from_plli2s,
        from_pin_i2s_ckin,
    };

    enum class rcc_source_for_mco1{
        from_hsi,
        from_lse,
        from_hse,
        from_pll,
    };

    enum class rcc_source_for_mco2{
        from_sclk,
        from_plli2s,
        from_hse,
        from_pll,
    };

    enum class rcc_source_for_rtc{
        none,
        from_lse,
        from_lsi,
        from_hse,
    };

    enum class rcc_drive_capability{
        low,
        middle_high, // middle_high is 1
        middle_low,  // middle_low is 2
        high,
    };

    enum class rcc_spread_mode{
        center,
        down,
    };

    struct none_t{
        constexpr none_t(){}
        xassume(rcc_for_rtc            , 0);
        xassume(rcc_for_mco            , 0);
        xassume(rcc_source_for_rtc     , 0);
    };

    struct div2_t{
        constexpr div2_t(){}
        xassume(rcc_for_rtc            , 2);
        xassume(rcc_for_mco            , 4);
        xassume(rcc_for_sclk           , 0);
        xassume(rcc_for_otg_sdio_random, 2);
    };

    struct div3_t{
        constexpr div3_t(){}
        xassume(rcc_for_rtc            , 3);
        xassume(rcc_for_mco            , 5);
        xassume(rcc_for_otg_sdio_random, 3);
    };

    struct div4_t{
        constexpr div4_t(){}
        xassume(rcc_for_rtc            , 4);
        xassume(rcc_for_mco            , 6);
        xassume(rcc_for_sclk           , 1);
        xassume(rcc_for_otg_sdio_random, 4);
    };

    struct div5_t{
        constexpr div5_t(){}
        xassume(rcc_for_rtc            , 5);
        xassume(rcc_for_mco            , 7);
        xassume(rcc_for_otg_sdio_random, 5);
    };

    struct div6_t{
        constexpr div6_t(){}
        xassume(rcc_for_rtc            , 6);
        xassume(rcc_for_sclk           , 2);
        xassume(rcc_for_otg_sdio_random, 6);
    };

    struct div7_t{
        constexpr div7_t(){}
        xassume(rcc_for_rtc            , 7);
        xassume(rcc_for_otg_sdio_random, 7);
    };

    struct div8_t{
        constexpr div8_t(){}
        xassume(rcc_for_rtc            , 8);
        xassume(rcc_for_sclk           , 3);
        xassume(rcc_for_otg_sdio_random, 8);
    };

    struct div9_15_t{
        constexpr div9_15_t(u32 value) : value(value) {}
        xassume(rcc_for_rtc            , value);
        xassume(rcc_for_otg_sdio_random, value);
    private:
        u32 value;
    };

    struct div16_31_t{
        constexpr div16_31_t(u32 value) : value(value) {}
        xassume(rcc_for_rtc, value);
    private:
        u32 value;
    };

    struct rcc_source_from_hsi{
        constexpr rcc_source_from_hsi(){}
        xassume(rcc_source_for_pll , 0);
        xassume(rcc_source_for_sclk, 0);
        xassume(rcc_source_for_mco1, 0);
    };

    struct rcc_source_from_hse{
        constexpr rcc_source_from_hse() {}
        xassume(rcc_source_for_pll , 1);
        xassume(rcc_source_for_rtc , 3);
        xassume(rcc_source_for_sclk, 1);
        xassume(rcc_source_for_mco1, 2);
        xassume(rcc_source_for_mco2, 2);
    };

    struct rcc_source_from_pll{
        constexpr rcc_source_from_pll(){}
        xassume(rcc_source_for_sclk, 2);
        xassume(rcc_source_for_mco1, 3);
        xassume(rcc_source_for_mco2, 3);
    };

    struct rcc_source_from_sclk{
        constexpr rcc_source_from_sclk() {}
        xassume(rcc_source_for_ahb, 0);
        xassume(rcc_source_for_mco2, 0);
    };

    struct rcc_source_from_sclk_div{
        constexpr rcc_source_from_sclk_div(u32 value) : value(value){}
        xassume(rcc_source_for_ahb, value);
    private:
        u32 value;
    };

    struct rcc_source_from_ahb{
        constexpr rcc_source_from_ahb(u32 value) : value(value) {}
        xassume(rcc_source_for_apb, value);
    private:
        u32 value;
    };

    struct rcc_source_from_plli2s{
        constexpr rcc_source_from_plli2s(){}
        xassume(rcc_source_for_i2s , 0);
        xassume(rcc_source_for_mco2, 1);
    };

    struct rcc_source_from_lse{
        constexpr rcc_source_from_lse(){}
        xassume(rcc_source_for_mco1, 1);
        xassume(rcc_source_for_rtc , 1);
    };

    enum class io_mode{
        //                                 mode   | od     | speed  | pull up/down
        ipt_floating                     = 0 << 5 | 0 << 4 | 0 << 2 | 0,
        ipt_pu                           = 0 << 5 | 0 << 4 | 0 << 2 | 1,
        ipt_pd                           = 0 << 5 | 0 << 4 | 0 << 2 | 2,

        opt_pp_at_low_speed              = 1 << 5 | 0 << 4 | 0 << 2 | 0,
        opt_pp_at_middle_speed           = 1 << 5 | 0 << 4 | 1 << 2 | 0,
        opt_pp_at_fast_speed             = 1 << 5 | 0 << 4 | 2 << 2 | 0,
        opt_pp_at_high_speed             = 1 << 5 | 0 << 4 | 3 << 2 | 0,
        opt_pp_pu_at_low_speed           = 1 << 5 | 0 << 4 | 0 << 2 | 1,
        opt_pp_pu_at_middle_speed        = 1 << 5 | 0 << 4 | 1 << 2 | 1,
        opt_pp_pu_at_fast_speed          = 1 << 5 | 0 << 4 | 2 << 2 | 1,
        opt_pp_pu_at_high_speed          = 1 << 5 | 0 << 4 | 3 << 2 | 1,
        opt_pp_pd_at_low_speed           = 1 << 5 | 0 << 4 | 0 << 2 | 2,
        opt_pp_pd_at_middle_speed        = 1 << 5 | 0 << 4 | 1 << 2 | 2,
        opt_pp_pd_at_fast_speed          = 1 << 5 | 0 << 4 | 2 << 2 | 2,
        opt_pp_pd_at_high_speed          = 1 << 5 | 0 << 4 | 3 << 2 | 2,

        opt_od_at_low_speed              = 1 << 5 | 1 << 4 | 0 << 2 | 0,
        opt_od_at_middle_speed           = 1 << 5 | 1 << 4 | 1 << 2 | 0,
        opt_od_at_fast_speed             = 1 << 5 | 1 << 4 | 2 << 2 | 0,
        opt_od_at_high_speed             = 1 << 5 | 1 << 4 | 3 << 2 | 0,
        opt_od_pu_at_low_speed           = 1 << 5 | 1 << 4 | 0 << 2 | 1,
        opt_od_pu_at_middle_speed        = 1 << 5 | 1 << 4 | 1 << 2 | 1,
        opt_od_pu_at_fast_speed          = 1 << 5 | 1 << 4 | 2 << 2 | 1,
        opt_od_pu_at_high_speed          = 1 << 5 | 1 << 4 | 3 << 2 | 1,
        opt_od_pd_at_low_speed           = 1 << 5 | 1 << 4 | 0 << 2 | 2,
        opt_od_pd_at_middle_speed        = 1 << 5 | 1 << 4 | 1 << 2 | 2,
        opt_od_pd_at_fast_speed          = 1 << 5 | 1 << 4 | 2 << 2 | 2,
        opt_od_pd_at_high_speed          = 1 << 5 | 1 << 4 | 3 << 2 | 2,

        afio_pp_at_low_speed             = 2 << 5 | 0 << 4 | 0 << 2 | 0,
        afio_pp_at_middle_speed          = 2 << 5 | 0 << 4 | 1 << 2 | 0,
        afio_pp_at_fast_speed            = 2 << 5 | 0 << 4 | 2 << 2 | 0,
        afio_pp_at_high_speed            = 2 << 5 | 0 << 4 | 3 << 2 | 0,
        afio_pp_pu_at_low_speed          = 2 << 5 | 0 << 4 | 0 << 2 | 1,
        afio_pp_pu_at_middle_speed       = 2 << 5 | 0 << 4 | 1 << 2 | 1,
        afio_pp_pu_at_fast_speed         = 2 << 5 | 0 << 4 | 2 << 2 | 1,
        afio_pp_pu_at_high_speed         = 2 << 5 | 0 << 4 | 3 << 2 | 1,
        afio_pp_pd_at_low_speed          = 2 << 5 | 0 << 4 | 0 << 2 | 2,
        afio_pp_pd_at_middle_speed       = 2 << 5 | 0 << 4 | 1 << 2 | 2,
        afio_pp_pd_at_fast_speed         = 2 << 5 | 0 << 4 | 2 << 2 | 2,
        afio_pp_pd_at_high_speed         = 2 << 5 | 0 << 4 | 3 << 2 | 2,

        afio_od_at_low_speed             = 2 << 5 | 1 << 4 | 0 << 2 | 0,
        afio_od_at_middle_speed          = 2 << 5 | 1 << 4 | 1 << 2 | 0,
        afio_od_at_fast_speed            = 2 << 5 | 1 << 4 | 2 << 2 | 0,
        afio_od_at_high_speed            = 2 << 5 | 1 << 4 | 3 << 2 | 0,
        afio_od_pu_at_low_speed          = 2 << 5 | 1 << 4 | 0 << 2 | 1,
        afio_od_pu_at_middle_speed       = 2 << 5 | 1 << 4 | 1 << 2 | 1,
        afio_od_pu_at_fast_speed         = 2 << 5 | 1 << 4 | 2 << 2 | 1,
        afio_od_pu_at_high_speed         = 2 << 5 | 1 << 4 | 3 << 2 | 1,
        afio_od_pd_at_low_speed          = 2 << 5 | 1 << 4 | 0 << 2 | 2,
        afio_od_pd_at_middle_speed       = 2 << 5 | 1 << 4 | 1 << 2 | 2,
        afio_od_pd_at_fast_speed         = 2 << 5 | 1 << 4 | 2 << 2 | 2,
        afio_od_pd_at_high_speed         = 2 << 5 | 1 << 4 | 3 << 2 | 2,

        analog                           = 3 << 5 | 0 << 4 | 3 << 2 | 0,
    };

    constexpr io_mode ipt_floating                      = io_mode::ipt_floating;
    constexpr io_mode ipt_pu                            = io_mode::ipt_pu;
    constexpr io_mode ipt_pd                            = io_mode::ipt_pd;
    constexpr io_mode opt_pp_at_low_speed               = io_mode::opt_pp_at_low_speed;
    constexpr io_mode opt_pp_at_middle_speed            = io_mode::opt_pp_at_middle_speed;
    constexpr io_mode opt_pp_at_fast_speed              = io_mode::opt_pp_at_fast_speed;
    constexpr io_mode opt_pp_at_high_speed              = io_mode::opt_pp_at_high_speed;
    constexpr io_mode opt_pp_pu_at_low_speed            = io_mode::opt_pp_pu_at_low_speed;
    constexpr io_mode opt_pp_pu_at_middle_speed         = io_mode::opt_pp_pu_at_middle_speed;
    constexpr io_mode opt_pp_pu_at_fast_speed           = io_mode::opt_pp_pu_at_fast_speed;
    constexpr io_mode opt_pp_pu_at_high_speed           = io_mode::opt_pp_pu_at_high_speed;
    constexpr io_mode opt_pp_pd_at_low_speed            = io_mode::opt_pp_pd_at_low_speed;
    constexpr io_mode opt_pp_pd_at_middle_speed         = io_mode::opt_pp_pd_at_middle_speed;
    constexpr io_mode opt_pp_pd_at_fast_speed           = io_mode::opt_pp_pd_at_fast_speed;
    constexpr io_mode opt_pp_pd_at_high_speed           = io_mode::opt_pp_pd_at_high_speed;
    constexpr io_mode opt_od_at_low_speed               = io_mode::opt_od_at_low_speed;
    constexpr io_mode opt_od_at_middle_speed            = io_mode::opt_od_at_middle_speed;
    constexpr io_mode opt_od_at_fast_speed              = io_mode::opt_od_at_fast_speed;
    constexpr io_mode opt_od_at_high_speed              = io_mode::opt_od_at_high_speed;
    constexpr io_mode opt_od_pu_at_low_speed            = io_mode::opt_od_pu_at_low_speed;
    constexpr io_mode opt_od_pu_at_middle_speed         = io_mode::opt_od_pu_at_middle_speed;
    constexpr io_mode opt_od_pu_at_fast_speed           = io_mode::opt_od_pu_at_fast_speed;
    constexpr io_mode opt_od_pu_at_high_speed           = io_mode::opt_od_pu_at_high_speed;
    constexpr io_mode opt_od_pd_at_low_speed            = io_mode::opt_od_pd_at_low_speed;
    constexpr io_mode opt_od_pd_at_middle_speed         = io_mode::opt_od_pd_at_middle_speed;
    constexpr io_mode opt_od_pd_at_fast_speed           = io_mode::opt_od_pd_at_fast_speed;
    constexpr io_mode opt_od_pd_at_high_speed           = io_mode::opt_od_pd_at_high_speed;
    constexpr io_mode afio_pp_at_low_speed              = io_mode::afio_pp_at_low_speed;
    constexpr io_mode afio_pp_at_middle_speed           = io_mode::afio_pp_at_middle_speed;
    constexpr io_mode afio_pp_at_fast_speed             = io_mode::afio_pp_at_fast_speed;
    constexpr io_mode afio_pp_at_high_speed             = io_mode::afio_pp_at_high_speed;
    constexpr io_mode afio_pp_pu_at_low_speed           = io_mode::afio_pp_pu_at_low_speed;
    constexpr io_mode afio_pp_pu_at_middle_speed        = io_mode::afio_pp_pu_at_middle_speed;
    constexpr io_mode afio_pp_pu_at_fast_speed          = io_mode::afio_pp_pu_at_fast_speed;
    constexpr io_mode afio_pp_pu_at_high_speed          = io_mode::afio_pp_pu_at_high_speed;
    constexpr io_mode afio_pp_pd_at_low_speed           = io_mode::afio_pp_pd_at_low_speed;
    constexpr io_mode afio_pp_pd_at_middle_speed        = io_mode::afio_pp_pd_at_middle_speed;
    constexpr io_mode afio_pp_pd_at_fast_speed          = io_mode::afio_pp_pd_at_fast_speed;
    constexpr io_mode afio_pp_pd_at_high_speed          = io_mode::afio_pp_pd_at_high_speed;
    constexpr io_mode afio_od_at_low_speed              = io_mode::afio_od_at_low_speed;
    constexpr io_mode afio_od_at_middle_speed           = io_mode::afio_od_at_middle_speed;
    constexpr io_mode afio_od_at_fast_speed             = io_mode::afio_od_at_fast_speed;
    constexpr io_mode afio_od_at_high_speed             = io_mode::afio_od_at_high_speed;
    constexpr io_mode afio_od_pu_at_low_speed           = io_mode::afio_od_pu_at_low_speed;
    constexpr io_mode afio_od_pu_at_middle_speed        = io_mode::afio_od_pu_at_middle_speed;
    constexpr io_mode afio_od_pu_at_fast_speed          = io_mode::afio_od_pu_at_fast_speed;
    constexpr io_mode afio_od_pu_at_high_speed          = io_mode::afio_od_pu_at_high_speed;
    constexpr io_mode afio_od_pd_at_low_speed           = io_mode::afio_od_pd_at_low_speed;
    constexpr io_mode afio_od_pd_at_middle_speed        = io_mode::afio_od_pd_at_middle_speed;
    constexpr io_mode afio_od_pd_at_fast_speed          = io_mode::afio_od_pd_at_fast_speed;
    constexpr io_mode afio_od_pd_at_high_speed          = io_mode::afio_od_pd_at_high_speed;
    constexpr io_mode analog                            = io_mode::analog;


    // SPI
    enum class spi_role{
        as_slaver,
        as_master,
    };

    enum class spi_baud_rate{
        from_apb_div2, from_apb_div4, from_apb_div8, from_apb_div16, from_apb_div32, from_apb_div64, from_apb_div128, from_apb_div256,
    };

    enum class spi_full_duplex{}   constexpr full_duplex{};
    enum class spi_half_duplex{}   constexpr half_duplex{};
    enum class spi_transmit_only{} constexpr transmit_only{};
    enum class spi_receive_only{}  constexpr receive_only{};

    enum class spi_crc_length{
        crc8,
        crc16,
    };

    enum class spi_bit_order{
        msb_first,
        lsb_first,
    };

    enum class spi_fifo_threshold{
        at_least_16bit,
        at_least_8bit,
    };

    enum class spi_frame_format{
        motorola_mode,
        ti_mode,
    };

    enum class spi_fifo_level{
        empty,
        one_fourth,
        half,
        full,
    };


}
