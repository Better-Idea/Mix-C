#pragma once
#include"definition/mini.hpp"
#define xflush        asm("":::"memory");
#define xro(type,name,target)                   \
    union {                                     \
        operator type(){                        \
            field * ptr = (field *)this;        \
            return type((u32)ptr->target);      \
        }                                       \
    } name;

#define xrw_core(type,target)                   \
    operator type(){                            \
            field * ptr = (field *)this;        \
            return type((u32)ptr->target);      \
        }                                       \
        void operator =(type value){            \
            field * ptr = (field *)this;        \
            ptr->target = u32(value);           \
        }                                       \

#define xrw(type,name,target)                   \
    union {                                     \
        xrw_core(type,target)                   \
    } name;

// read/clear/enable/disable
#define xevent(name,flag,clear,enable)          \
    union {                                     \
        operator bool(){                        \
            field * ptr = (field *)this;        \
            return bool((u32)ptr->flag);        \
        }                                       \
        void operator=(bool value){             \
            field * ptr = (field *)this;        \
            ptr->enable = value;                \
        }                                       \
        void operator=(clear_t value){          \
            field * ptr = (field *)this;        \
            ptr->clear = 1;                     \
        }                                       \
    } name;

#define xrwpair(type,target)                    \
    operator type(){                            \
        field * ptr = (field *)this;            \
        return type((u32)ptr->target);          \
    }                                           \
    void operator()(type value){                \
        field * ptr = (field *)this;            \
        ptr->target = u32(value);               \
    }                                           \

#define xgroup_set()                            \
    template<class a, class b, class ... args>  \
    void operator()(a a0, b a1, args ... rest) {\
        thex(a0);                               \
        thex(a1, rest...);                      \
    }                                           \

// - enable/disable
// - keep_reset/keep_variable
// - low power configure
#define xport(name)                             \
    union {                                     \
        xrwpair(port, xlink(name, rst));        \
        xrwpair(bool, xlink(name, en));         \
        xrwpair(low_power_mode,                 \
            xlink(name, lpen));                 \
        xgroup_set()                            \
    } name;

// - keep_reset/keep_variable
// - low power configure
#define xport_en_lpen(name)                     \
    union {                                     \
        xrwpair(port, xlink(name, en));         \
        xrwpair(low_power_mode,                 \
            xlink(name, lpen));                 \
        xgroup_set()                            \
    } name;

// - low power configure
#define xport_lpen(name)                        \
    union {                                     \
        xrwpair(low_power_mode,                 \
            xlink(name, lpen));                 \
    } name;


// - keep_reset/keep_variable
#define xport_rst(name)                         \
    union {                                     \
        xrwpair(port,                           \
            xlink(name, rst));                  \
    } name;

#define xassume(type,value)                     \
    xcvtc(type)                                 \
        return type(value);                     \
    $

xstruct(clear_t) 
    constexpr clear_t(){} 
$;

xenumc(source_regulator)
    from_main_voltage_regulator,
    from_low_power_voltage_regulator,
$;

xenumc(deep_sleep)
    enter_stop_mode,
    enter_standby_mode,
$;

xenumc(thresholds_voltage)
    at_2v0,
    at_2v1,
    at_2v3, // no 2v4
    at_2v5,
    at_2v6,
    at_2v7,
    at_2v8,
    at_2v9,
$;

xenumc(backup_domain)
    enable_write_protection,
    disable_write_protection,
$;

xenumc(flash)
    keep_power,
    power_down,
$;

xenumc(regulator)
    keep_enable,
    enter_under_drive_mode,
$;

xenumc(regulator_scaling)
    lv3 = 1, // reverse order
    lv2,
    lv1
$;

constexpr source_regulator      from_main_voltage_regulator         = source_regulator::from_main_voltage_regulator;
constexpr source_regulator      from_low_power_voltage_regulator    = source_regulator::from_low_power_voltage_regulator;
constexpr deep_sleep            enter_stop_mode                     = deep_sleep::enter_stop_mode;
constexpr deep_sleep            enter_standby_mode                  = deep_sleep::enter_standby_mode;
constexpr thresholds_voltage    at_2v0                              = thresholds_voltage::at_2v0;
constexpr thresholds_voltage    at_2v1                              = thresholds_voltage::at_2v1;
constexpr thresholds_voltage    at_2v3                              = thresholds_voltage::at_2v3;
constexpr thresholds_voltage    at_2v5                              = thresholds_voltage::at_2v5;
constexpr thresholds_voltage    at_2v6                              = thresholds_voltage::at_2v6;
constexpr thresholds_voltage    at_2v7                              = thresholds_voltage::at_2v7;
constexpr thresholds_voltage    at_2v8                              = thresholds_voltage::at_2v8;
constexpr thresholds_voltage    at_2v9                              = thresholds_voltage::at_2v9;
constexpr backup_domain         enable_write_protection             = backup_domain::enable_write_protection;
constexpr backup_domain         disable_write_protection            = backup_domain::disable_write_protection;
constexpr flash                 keep_power                          = flash::keep_power;
constexpr flash                 power_down                          = flash::power_down;
constexpr regulator             keep_enable                         = regulator::keep_enable;
constexpr regulator             enter_under_drive_mode              = regulator::enter_under_drive_mode;
constexpr regulator_scaling     lv3                                 = regulator_scaling::lv3;
constexpr regulator_scaling     lv2                                 = regulator_scaling::lv2;
constexpr regulator_scaling     lv1                                 = regulator_scaling::lv1;

xunion(_pwr)
private:
    xstruct(field)
        u32 lpds        : 1;
        u32 pdds        : 1;
        xr32(1);
        u32 csbf        : 1;
        u32 pvde        : 1;
        u32 pls         : 3;
        u32 dbp         : 1;
        u32 fpds        : 1;
        u32 lpuds       : 1;
        u32 mruds       : 1;
        xr32(1);
        u32 adcdc1      : 1;
        u32 vos         : 2;
        u32 oden        : 1;
        u32 odswen      : 1;
        u32 uden        : 2;

        u32 wuif        : 1;
        u32 sbf         : 1;
        u32 pvdo        : 1;
        u32 brr         : 1;
        u32 eiwup       : 1;
        xr32(4);
        u32 bre         : 1;
        xr32(4);
        u32 vosrdy      : 1;
        xr32(1);
        u32 odrdy       : 1;
        u32 odswrdy     : 1;
        u32 udrdy       : 2;
        xr32(12);

        u32 cwupf1      : 1;
        u32 cwupf2      : 1;
        u32 cwupf3      : 1;
        u32 cwupf4      : 1;
        u32 cwupf5      : 1;
        u32 cwupf6      : 1;
        xr32(2);
        u32 wupp1       : 1;
        u32 wupp2       : 1;
        u32 wupp3       : 1;
        u32 wupp4       : 1;
        u32 wupp5       : 1;
        u32 wupp6       : 1;
        xr32(2 + 16);

        u32 wupf1       : 1;
        u32 wupf2       : 1;
        u32 wupf3       : 1;
        u32 wupf4       : 1;
        u32 wupf5       : 1;
        u32 wupf6       : 1;
        xr32(2);
        u32 ewup1       : 1;
        u32 ewup2       : 1;
        u32 ewup3       : 1;
        u32 ewup4       : 1;
        u32 ewup5       : 1;
        u32 ewup6       : 1;
        xr32(2 + 16);
    $ items;

public:
    xunion()
        xunion()
            xrw(::source_regulator, source, lpds);
            xrw(::regulator, low_power, lpuds);
            xrw(::regulator, main, mruds);
        $ regulator;

        xrw(::flash, flash, fpds);

        xunion()
            xunion()
                xopc(=, bool value)
                    field * ptr = (field *)this;
                    ptr->uden = value ? 0x3 : 0; // value 1 and 2 are reserved
                $

                xcvtc(bool)
                    field * ptr = (field *)this;
                    return bool(ptr->uden != 0);
                $
            $ enable;

            xrw(bool, ready, udrdy);
        $ under_drive;
    $ stop_mode;

    xrw(::deep_sleep, deep_sleep, pdds);
    
    // TODO:standby_flag csbf -------------------------------
    // xrw(clear_t, standby_flag, csbf);

    // power voltage detector
    xunion()
        xrw(bool, enable, pvde);
        xrw(::thresholds_voltage, thresholds_voltage, pls);
        xro(bool, higher_than_vdd, pvdo);
    $ pvd;

    xrw(::backup_domain, backup_domain, dbp);

    // TODO:ADCDC1
    // xrw(xx, adcdc1, adcdc1);

    xunion()
        // can be modifed only when PLL is OFF
        // active at PLL ON
        xrw(::regulator_scaling, at, vos);

        xro(bool, ready, vosrdy);
    $ regulator_scaling;

    xunion()
        // HSI or HSE must be selected as system clock before set this bit
        xrw(bool, enable, oden);
        xro(bool, ready, odrdy);

        xunion()
            // set this bit to switching to over drive mode
            // need wait over_drive.ready bit equals to 1 before
            xrw(bool, enable, odswen);
            xro(bool, ready, odswrdy);
        $ switching;
    $ over_drive;

    xro(bool, standby, sbf);

    xunion()
        xro(bool, occur, wuif);
        xrw(bool, enable, eiwup);
    $ wakeup_internal;

    xunion()
        xro(bool, ready, brr);

        // used to maintain backup SRAM content in standby and vbat modes
        // the content which in backup SRAM will lose when disable the backup_regulator.
        xrw(bool, enable, bre);
    $ backup_regulator;
$;




xenumc(port)
    keep_variable,
    keep_reset,
$;

xenumc(low_power_mode)
    close_clock_when_sleep,
    keep_clock,
$;

xenumc(for_rtc)
    none,
    div2 = 2, div3, div4, div5, div6, div7, div8, div9, div10, div11, div12, div13, div14, div15, div16, div17, div18, div19, div20, div21, div22, div23, div24, div25, div26, div27, div28, div29, div30, div31,
$;

xenumc(for_mco)
    none,
    div2 = 4, div3, div4, div5,
$;

xenumc(for_sclk)
    div2, div4, div6, div8,
$;

xenumc(for_otg_sdio_random)
    div2 = 2, div3, div4, div5, div6, div7, div8, div9, div10, div11, div12, div13, div14, div15,
$;

xenumc(source_for_pll)
    from_hsi,
    from_hse,
$;

xenumc(source_for_sclk)
    from_hsi,
    from_hse,
    from_pll,
$;

xenumc(source_for_ahb)
    from_sclk,
    from_sclk_div2 = 8,
    from_sclk_div4,
    from_sclk_div8,
    from_sclk_div16,
    from_sclk_div64,
    from_sclk_div128,
    from_sclk_div256,
    from_sclk_div512,
$;

xenumc(source_for_apb)
    from_ahb,
    from_ahb_div2 = 4,
    from_ahb_div4,
    from_ahb_div8,
    from_ahb_div16,
$;

xenumc(source_for_i2s)
    from_plli2s,
    from_pin_i2s_ckin,
$;

xenumc(source_for_mco1)
    from_hsi,
    from_lse,
    from_hse,
    from_pll,
$;

xenumc(source_for_mco2)
    from_sclk,
    from_plli2s,
    from_hse,
    from_pll,
$;

xenumc(source_for_rtc)
    none,
    from_lse,
    from_lsi,
    from_hse,
$;

xenumc(drive_capability)
    low,
    middle_high, // middle_high is 1
    middle_low,  // middle_low is 2
    high,
$;

xenumc(spread_mode)
    center,
    down,
$;

xstruct(none_t)
    constexpr none_t(){}
    xassume(for_rtc            , 0);
    xassume(for_mco            , 0);
    xassume(source_for_rtc     , 0);
$;

xstruct(div2_t)
    constexpr div2_t(){}
    xassume(for_rtc            , 2);
    xassume(for_mco            , 4);
    xassume(for_sclk           , 0);
    xassume(for_otg_sdio_random, 2);
$;

xstruct(div3_t)
    constexpr div3_t(){}
    xassume(for_rtc            , 3);
    xassume(for_mco            , 5);
    xassume(for_otg_sdio_random, 3);
$;

xstruct(div4_t)
    constexpr div4_t(){}
    xassume(for_rtc            , 4);
    xassume(for_mco            , 6);
    xassume(for_sclk           , 1);
    xassume(for_otg_sdio_random, 4);
$;

xstruct(div5_t)
    constexpr div5_t(){}
    xassume(for_rtc            , 5);
    xassume(for_mco            , 7);
    xassume(for_otg_sdio_random, 5);
$;

xstruct(div6_t)
    constexpr div6_t(){}
    xassume(for_rtc            , 6);
    xassume(for_sclk           , 2);
    xassume(for_otg_sdio_random, 6);
$;

xstruct(div7_t)
    constexpr div7_t(){}
    xassume(for_rtc            , 7);
    xassume(for_otg_sdio_random, 7);
$;

xstruct(div8_t)
    constexpr div8_t(){}
    xassume(for_rtc            , 8);
    xassume(for_sclk           , 3);
    xassume(for_otg_sdio_random, 8);
$;

xstruct(div9_15_t)
    constexpr div9_15_t(u32 value) : value(value) {}
    xassume(for_rtc            , value);
    xassume(for_otg_sdio_random, value);
private:
    u32 value;
$;

xstruct(div16_31_t)
    constexpr div16_31_t(u32 value) : value(value) {}
    xassume(for_rtc, value);
private:
    u32 value;
$;

xstruct(source_from_hsi)
    constexpr source_from_hsi(){}
    xassume(source_for_pll , 0);
    xassume(source_for_sclk, 0);
    xassume(source_for_mco1, 0);
$;

xstruct(source_from_hse)
    constexpr source_from_hse() {}
    xassume(source_for_pll , 1);
    xassume(source_for_rtc , 3);
    xassume(source_for_sclk, 1);
    xassume(source_for_mco1, 2);
    xassume(source_for_mco2, 2);
$;

xstruct(source_from_pll)
    constexpr source_from_pll(){}
    xassume(source_for_sclk, 2);
    xassume(source_for_mco1, 3);
    xassume(source_for_mco2, 3);
$;

xstruct(source_from_sclk)
    constexpr source_from_sclk() {}
    xassume(source_for_ahb, 0);
    xassume(source_for_mco2, 0);
$;

xstruct(source_from_sclk_div)
    constexpr source_from_sclk_div(u32 value) : value(value){}
    xassume(source_for_ahb, value);
private:
    u32 value;
$;

xstruct(source_from_ahb)
    constexpr source_from_ahb(u32 value) : value(value) {}
    xassume(source_for_apb, value);
private:
    u32 value;
$;

xstruct(source_from_plli2s)
    constexpr source_from_plli2s(){}
    xassume(source_for_i2s , 0);
    xassume(source_for_mco2, 1);
$;

xstruct(source_from_lse)
    constexpr source_from_lse(){}
    xassume(source_for_mco1, 1);
    xassume(source_for_rtc, 1);
$;

constexpr port                  keep_variable           = port::keep_variable;
constexpr port                  keep_reset              = port::keep_reset;
constexpr low_power_mode        keep_clock              = low_power_mode::keep_clock;
constexpr low_power_mode        close_clock_when_sleep  = low_power_mode::close_clock_when_sleep;
constexpr clear_t               clear {};
constexpr bool                  enable          = true;
constexpr bool                  disable         = false;
constexpr div2_t                div2 {};
constexpr div3_t                div3 {};
constexpr div4_t                div4 {};
constexpr div5_t                div5 {};
constexpr div6_t                div6 {};
constexpr div7_t                div7 {};
constexpr div8_t                div8 {};
constexpr div9_15_t             div9 {9}, div10 {10}, div11 {11}, div12 {12}, div13 {13}, div14 {14}, div15 {15};
constexpr div16_31_t            div16 {16}, div17 {17}, div18 {18}, div19 {19}, div20 {20}, div21{21}, div22 {22}, div23 {23}, div24 {24}, div25 {25}, div26 {26}, div27 {27}, div28 {28}, div29 {29}, div30 {30}, div31 {31};
constexpr source_from_sclk      from_sclk {};
constexpr source_from_sclk_div  from_sclk_div2 {8}, from_sclk_div4 {9}, from_sclk_div8 {10}, from_sclk_div16 {11}, from_sclk_div64 {12}, from_sclk_div128 {13}, from_sclk_div256 {14}, from_sclk_div512 {15};
constexpr source_from_ahb       from_ahb {0}, from_ahb_div2 {4}, from_ahb_div4 {5}, from_ahb_div8 {6}, from_ahb_div16{7};
constexpr source_from_hsi       from_hsi {};
constexpr source_from_hse       from_hse {};
constexpr source_from_pll       from_pll {};
constexpr source_from_plli2s    from_plli2s {};
constexpr source_for_i2s        from_pin_i2s_ckin       = source_for_i2s::from_pin_i2s_ckin;
constexpr source_for_rtc        from_lsi                = source_for_rtc::from_lsi;
constexpr drive_capability      low                     = drive_capability::low;
constexpr drive_capability      middle_high             = drive_capability::middle_high;
constexpr drive_capability      middle_low              = drive_capability::middle_low;
constexpr drive_capability      high                    = drive_capability::high;
constexpr spread_mode           center                  = spread_mode::center;
constexpr spread_mode           down                    = spread_mode::down;

xunion(_rcc)
private:
    xstruct(field)
        u32 hsion       : 1;
        u32 hsirdy      : 1;
        xr32(1);
        u32 hsitrim     : 5;
        u32 hsical      : 8;
        u32 hseon       : 1;
        u32 hserdy      : 1;
        u32 hsebyp      : 1;
        u32 csson       : 1;
        xr32(4);
        u32 pllon       : 1;
        u32 pllrdy      : 1;
        u32 plli2son    : 1;
        u32 plli2srdy   : 1;
        u32 pllsaion    : 1;
        u32 pllsairdy   : 1;
        xr32(2);

        u32 pllm        : 6;
        u32 plln        : 9;
        xr32(1);
        u32 pllp        : 2;
        xr32(4);
        u32 pllsrc      : 1;
        xr32(1);
        u32 pllq        : 4;
        xr32(4);

        u32 sw          : 2;
        u32 sws         : 2;
        u32 hpre        : 4;
        xr32(2);
        u32 ppre1       : 3;
        u32 ppre2       : 3;
        u32 rtcpre      : 5;
        u32 mco1        : 2;
        u32 i2ssrc      : 1;
        u32 mco1pre     : 3;
        u32 mco2pre     : 3;
        u32 mco2        : 2;

        u32 lsirdyf     : 1;
        u32 lserdyf     : 1;
        u32 hsirdyf     : 1;
        u32 hserdyf     : 1;
        u32 pllrdyf     : 1;
        u32 plli2srdyf  : 1;
        u32 pllsairdyf  : 1;
        u32 cssf        : 1;
        u32 lsirdyie    : 1;
        u32 lserdyie    : 1;
        u32 hsirdyie    : 1;
        u32 hserdyie    : 1;
        u32 pllrdyie    : 1;
        u32 plli2srdyie : 1;
        u32 pllsairdyie : 1;
        xr32(1);
        u32 lsirdyc     : 1;
        u32 lserdyc     : 1;
        u32 hsirdyc     : 1;
        u32 hserdyc     : 1;
        u32 pllrdyc     : 1;
        u32 plli2srdyc  : 1;
        u32 pllsairdyc  : 1;
        u32 cssc        : 1;

        #define xarg 0
        #include"core.rcc.gen.hpp"

        #define xarg 1
        #include"core.rcc.gen.hpp"

        #define xarg 2
        #include"core.rcc.gen.hpp"

        u32 lseon       : 1;
        u32 lserdy      : 1;
        u32 lsebyp      : 1;
        u32 lsedrv      : 2;
        xr32(3);
        u32 rtcsel      : 2;
        xr32(5);
        u32 rtcen       : 1;
        u32 bdrst       : 1;
        xr32(15);

        u32 lsion       : 1;
        u32 lsirdy      : 1;
        xr32(14 + 8);
        u32 rmvf        : 1;
        u32 borrstf     : 1;
        u32 pinrstf     : 1;
        u32 porrstf     : 1;
        u32 sftrstf     : 1;
        u32 iwdgrstf    : 1;
        u32 wwdgrstf    : 1;
        u32 lpwrrstf    : 1;

        xr32(32);
        xr32(32);

        u32 modper      : 13;
        u32 incstep     : 15;
        xr32(2);
        u32 spreadsel   : 1;
        u32 sscgen      : 1;

        // to be continue...
    $ items;

public:
    xrw(bool, reset_backup_domain, bdrst);

    // TODO:comment
    xunion()
        xrw(u32, period, modper);
        xrw(u32, step, incstep);
        xrw(spread_mode, mode, spreadsel);
        xrw(bool, enable, sscgen);
    $ spread_spectrum;

    xunion(_flag)
        xro(bool, bor_reset, borrstf);
        xro(bool, pin_reset, pinrstf);
        xro(bool, por_reset, porrstf);
        xro(bool, software_reset, sftrstf);
        xro(bool, iwdg_reset, iwdgrstf);
        xro(bool, wwdg_reset, wwdgrstf);
        xro(bool, lpwr_reset, lpwrrstf);
        xdef(clear_all)
            remove_reset_flag = true;
        $
    private:
        xrw(bool, remove_reset_flag, rmvf);
    $ flag;

    xunion()
        xrw(bool, enable, lseon);
        xro(bool, ready, lserdy);
        xrw(bool, bypass, lsebyp);
        xrw(::drive_capability, drive_capability, lsedrv);
    $ lse;

    xunion()
        xrw(bool, enable, lsion);
        xro(bool, ready, lsirdy);
    $ lsi;

    xunion()
        xrw(bool, enable, hsion);
        xro(bool, ready, hsirdy);
        xrw(u32 , triming, hsitrim);
        xro(u32 , calibration, hsical);
    $ hsi;

    xunion()
        xrw(bool, enable, hseon);
        xrw(bool, ready, hserdy);
        xrw(bool, bypass, hsebyp);
        xrw(bool, safe_clock, csson);

        // divsion from hse
        // rtc clock need equals to 1MHz
        xrw(::for_rtc, for_rtc, rtcpre);
    $ hse;

    xunion()
        xrw(bool, enable, plli2son);
        xrw(bool, ready, plli2srdy);
        xrw(source_for_i2s, source, i2ssrc);
    $ i2s;

    xunion()
        xrw(bool, enable, pllsaion);
        xrw(bool, ready, pllsairdy);
    $ sai;

    xunion()
        xrw(bool, enable, pllon);
        xro(bool, ready, pllrdy);

        // f(vco) = f(pll input) * (mul / div)
        // f(pll normal) = f(vco) / for_sclk
        // f(usb otg fs/sdio/random) = f(vco) / for_otg_sdio_random
        xunion()
            // division for vco input frequency
            // notice:
            // div must in range [2, 63]
            xrw(u32, div, pllm);

            // multiplier for vco output frequency
            // notice:
            // vco input freq * mul must in range [192, 432] MHz
            xrw(u32, mul, plln);

            // division for main system clock(sclk) when pll as the sclk
            // notice:
            // make sure sclk freqence no more than 180MHz
            // only 2, 4, 6, 8 is legal
            xrw(::for_sclk, for_sclk, pllp);

            // division for usb otg fs/sdio/random
            // notice:
            // usb otg fs need 48MHz frequence
            // sdio/random need less equal than 48MHz
            // for_otg_sdio_random must in range [2, 15]
            xrw(::for_otg_sdio_random, for_otg_sdio_random, pllq);
        $ freq;

        xrw(source_for_pll, source, pllsrc);
    $ pll;

    xunion()
        xrw(source_for_sclk, source, sw);
        xro(source_for_sclk, current_source, sws);
    $ sclk;

    xunion()
        // division from sclk
        // ahb clock not less than 25MHz
        xrw(source_for_ahb, source, hpre);
    $ ahb;

    xunion()
        // division from ahb
        // apb clock no more than 45MHz
        xrw(source_for_apb, source, ppre1);
    $ apb1;

    xunion()
        // division from ahb
        // apb clock no more than 90MHz
        xrw(source_for_apb, source, ppre2);
    $ apb2;

    xunion()
        xrw(bool, enable, rtcen);
        xrw(source_for_rtc, source, rtcsel);
    $ rtc;

    xunion()
        xrw(source_for_mco1, source, mco1);
        xrw(for_mco, freq, mco1pre);
    $ mco1;

    xunion()
        xrw(source_for_mco2, source, mco2);
        xrw(for_mco, freq, mco2pre);
    $ mco2;

    // those property can be set clear/enable/disable
    // and read for check whether the event is occur.
    xunion()
        xevent(lsi_ready   , lsirdyf   , lsirdyc   , lsirdyie);
        xevent(lse_ready   , lserdyf   , lserdyc   , lserdyie);
        xevent(hsi_ready   , hsirdyf   , hsirdyc   , hsirdyie);
        xevent(hse_ready   , hserdyf   , hserdyc   , hserdyie);
        xevent(pll_ready   , pllrdyf   , pllrdyc   , pllrdyie);
        xevent(plli2s_ready, plli2srdyf, plli2srdyc, plli2srdyie);
        xevent(pllsai_ready, pllsairdyf, pllsairdyc, pllsairdyie);
        xevent(hse_broken  , cssf      , cssc      , csson);

        xdef(clear_all)
            lsi_ready    = clear;
            lse_ready    = clear;
            hsi_ready    = clear;
            hse_ready    = clear;
            pll_ready    = clear;
            plli2s_ready = clear;
            pllsai_ready = clear;
            hse_broken   = clear;
        $
    $ event;

    xport(gpioa);
    xport(gpiob);
    xport(gpioc);
    xport(gpiod);
    xport(gpioe);
    xport(gpiof);
    xport(gpiog);
    xport(gpioh);
    xport(gpioi);
    xport(gpioj);
    xport(gpiok);
    xport(crc);

    xport_lpen(axi);
    xport_lpen(flash);
    xport_lpen(sram1);
    xport_lpen(sram2);

    xport_en_lpen(backup_sram);
    xport_en_lpen(tdcm_ram);

    xport(dma1);
    xport(dma2);
    xport(dma2d);
    xport(eth);
    xport_en_lpen(eth_tx);
    xport_en_lpen(eth_rx);
    xport_en_lpen(eth_ptp);
    xport(otghs);
    xport_en_lpen(otghs_ulpi);
    xport(dcmi);
    xport(crypto);
    xport(hash);
    xport(random);
    xport(otgfs);
    xport(fmc);
    xport(qspi);
    xport(tim2);
    xport(tim3);
    xport(tim4);
    xport(tim5);
    xport(tim6);
    xport(tim7);
    xport(tim12);
    xport(tim13);
    xport(tim14);
    xport(lptim1);
    xport(wwdg);
    xport(spi2);
    xport(spi3);
    xport(spdif_rx);
    xport(uart2);
    xport(uart3);
    xport(uart4);
    xport(uart5);
    xport(i2c1);
    xport(i2c2);
    xport(i2c3);
    xport(i2c4);
    xport(can1);
    xport(can2);
    xport(cec);
    xport(pwr);
    xport(dac);
    xport(uart7);
    xport(uart8);
    xport(tim1);
    xport(tim8);
    xport(usart1);
    xport(usart6);
    
    xport_rst(adc);
    xport_en_lpen(adc1);
    xport_en_lpen(adc2);
    xport_en_lpen(adc3);

    xport(sdmmc1);
    xport(spi1);
    xport(spi4);
    xport(syscfg);
    xport(tim9);
    xport(tim10);
    xport(tim11);
    xport(spi5);
    xport(spi6);
    xport(sai1);
    xport(sai2);
    xport(ltdc);
$;

xenumc(io_mode)
    //                                 mode   | od     | speed  | pull up/down
    ipt_floating                     = 0 << 5 | 0 << 4 | 0 << 2 | 0,
    ipt_pu                           = 0 << 5 | 0 << 4 | 0 << 2 | 1,
    ipt_pd                           = 0 << 5 | 0 << 4 | 0 << 2 | 2,

    opt_pp_at_low_speed              = 1 << 5 | 0 << 4 | 0 << 2 | 0,
    opt_pp_at_middle_speed           = 1 << 5 | 0 << 4 | 1 << 2 | 0,
    opt_pp_at_fast_speed             = 1 << 5 | 0 << 4 | 2 << 2 | 0,
    opt_pp_at_high_speed             = 1 << 5 | 0 << 4 | 3 << 2 | 0,
    opt_pp_with_pu_at_low_speed      = 1 << 5 | 0 << 4 | 0 << 2 | 1,
    opt_pp_with_pu_at_middle_speed   = 1 << 5 | 0 << 4 | 1 << 2 | 1,
    opt_pp_with_pu_at_fast_speed     = 1 << 5 | 0 << 4 | 2 << 2 | 1,
    opt_pp_with_pu_at_high_speed     = 1 << 5 | 0 << 4 | 3 << 2 | 1,
    opt_pp_with_pd_at_low_speed      = 1 << 5 | 0 << 4 | 0 << 2 | 2,
    opt_pp_with_pd_at_middle_speed   = 1 << 5 | 0 << 4 | 1 << 2 | 2,
    opt_pp_with_pd_at_fast_speed     = 1 << 5 | 0 << 4 | 2 << 2 | 2,
    opt_pp_with_pd_at_high_speed     = 1 << 5 | 0 << 4 | 3 << 2 | 2,

    opt_od_at_low_speed              = 1 << 5 | 1 << 4 | 0 << 2 | 0,
    opt_od_at_middle_speed           = 1 << 5 | 1 << 4 | 1 << 2 | 0,
    opt_od_at_fast_speed             = 1 << 5 | 1 << 4 | 2 << 2 | 0,
    opt_od_at_high_speed             = 1 << 5 | 1 << 4 | 3 << 2 | 0,
    opt_od_with_pu_at_low_speed      = 1 << 5 | 1 << 4 | 0 << 2 | 1,
    opt_od_with_pu_at_middle_speed   = 1 << 5 | 1 << 4 | 1 << 2 | 1,
    opt_od_with_pu_at_fast_speed     = 1 << 5 | 1 << 4 | 2 << 2 | 1,
    opt_od_with_pu_at_high_speed     = 1 << 5 | 1 << 4 | 3 << 2 | 1,
    opt_od_with_pd_at_low_speed      = 1 << 5 | 1 << 4 | 0 << 2 | 2,
    opt_od_with_pd_at_middle_speed   = 1 << 5 | 1 << 4 | 1 << 2 | 2,
    opt_od_with_pd_at_fast_speed     = 1 << 5 | 1 << 4 | 2 << 2 | 2,
    opt_od_with_pd_at_high_speed     = 1 << 5 | 1 << 4 | 3 << 2 | 2,

    afio_pp_at_low_speed             = 2 << 5 | 0 << 4 | 0 << 2 | 0,
    afio_pp_at_middle_speed          = 2 << 5 | 0 << 4 | 1 << 2 | 0,
    afio_pp_at_fast_speed            = 2 << 5 | 0 << 4 | 2 << 2 | 0,
    afio_pp_at_high_speed            = 2 << 5 | 0 << 4 | 3 << 2 | 0,
    afio_pp_with_pu_at_low_speed     = 2 << 5 | 0 << 4 | 0 << 2 | 1,
    afio_pp_with_pu_at_middle_speed  = 2 << 5 | 0 << 4 | 1 << 2 | 1,
    afio_pp_with_pu_at_fast_speed    = 2 << 5 | 0 << 4 | 2 << 2 | 1,
    afio_pp_with_pu_at_high_speed    = 2 << 5 | 0 << 4 | 3 << 2 | 1,
    afio_pp_with_pd_at_low_speed     = 2 << 5 | 0 << 4 | 0 << 2 | 2,
    afio_pp_with_pd_at_middle_speed  = 2 << 5 | 0 << 4 | 1 << 2 | 2,
    afio_pp_with_pd_at_fast_speed    = 2 << 5 | 0 << 4 | 2 << 2 | 2,
    afio_pp_with_pd_at_high_speed    = 2 << 5 | 0 << 4 | 3 << 2 | 2,

    afio_od_at_low_speed             = 2 << 5 | 1 << 4 | 0 << 2 | 0,
    afio_od_at_middle_speed          = 2 << 5 | 1 << 4 | 1 << 2 | 0,
    afio_od_at_fast_speed            = 2 << 5 | 1 << 4 | 2 << 2 | 0,
    afio_od_at_high_speed            = 2 << 5 | 1 << 4 | 3 << 2 | 0,
    afio_od_with_pu_at_low_speed     = 2 << 5 | 1 << 4 | 0 << 2 | 1,
    afio_od_with_pu_at_middle_speed  = 2 << 5 | 1 << 4 | 1 << 2 | 1,
    afio_od_with_pu_at_fast_speed    = 2 << 5 | 1 << 4 | 2 << 2 | 1,
    afio_od_with_pu_at_high_speed    = 2 << 5 | 1 << 4 | 3 << 2 | 1,
    afio_od_with_pd_at_low_speed     = 2 << 5 | 1 << 4 | 0 << 2 | 2,
    afio_od_with_pd_at_middle_speed  = 2 << 5 | 1 << 4 | 1 << 2 | 2,
    afio_od_with_pd_at_fast_speed    = 2 << 5 | 1 << 4 | 2 << 2 | 2,
    afio_od_with_pd_at_high_speed    = 2 << 5 | 1 << 4 | 3 << 2 | 2,

    analog                           = 3 << 5 | 0 << 4 | 3 << 2 | 0,
$;

constexpr io_mode ipt_floating                      = io_mode::ipt_floating;
constexpr io_mode ipt_pu                            = io_mode::ipt_pu;
constexpr io_mode ipt_pd                            = io_mode::ipt_pd;
constexpr io_mode opt_pp_at_low_speed               = io_mode::opt_pp_at_low_speed;
constexpr io_mode opt_pp_at_middle_speed            = io_mode::opt_pp_at_middle_speed;
constexpr io_mode opt_pp_at_fast_speed              = io_mode::opt_pp_at_fast_speed;
constexpr io_mode opt_pp_at_high_speed              = io_mode::opt_pp_at_high_speed;
constexpr io_mode opt_pp_with_pu_at_low_speed       = io_mode::opt_pp_with_pu_at_low_speed;
constexpr io_mode opt_pp_with_pu_at_middle_speed    = io_mode::opt_pp_with_pu_at_middle_speed;
constexpr io_mode opt_pp_with_pu_at_fast_speed      = io_mode::opt_pp_with_pu_at_fast_speed;
constexpr io_mode opt_pp_with_pu_at_high_speed      = io_mode::opt_pp_with_pu_at_high_speed;
constexpr io_mode opt_pp_with_pd_at_low_speed       = io_mode::opt_pp_with_pd_at_low_speed;
constexpr io_mode opt_pp_with_pd_at_middle_speed    = io_mode::opt_pp_with_pd_at_middle_speed;
constexpr io_mode opt_pp_with_pd_at_fast_speed      = io_mode::opt_pp_with_pd_at_fast_speed;
constexpr io_mode opt_pp_with_pd_at_high_speed      = io_mode::opt_pp_with_pd_at_high_speed;
constexpr io_mode opt_od_at_low_speed               = io_mode::opt_od_at_low_speed;
constexpr io_mode opt_od_at_middle_speed            = io_mode::opt_od_at_middle_speed;
constexpr io_mode opt_od_at_fast_speed              = io_mode::opt_od_at_fast_speed;
constexpr io_mode opt_od_at_high_speed              = io_mode::opt_od_at_high_speed;
constexpr io_mode opt_od_with_pu_at_low_speed       = io_mode::opt_od_with_pu_at_low_speed;
constexpr io_mode opt_od_with_pu_at_middle_speed    = io_mode::opt_od_with_pu_at_middle_speed;
constexpr io_mode opt_od_with_pu_at_fast_speed      = io_mode::opt_od_with_pu_at_fast_speed;
constexpr io_mode opt_od_with_pu_at_high_speed      = io_mode::opt_od_with_pu_at_high_speed;
constexpr io_mode opt_od_with_pd_at_low_speed       = io_mode::opt_od_with_pd_at_low_speed;
constexpr io_mode opt_od_with_pd_at_middle_speed    = io_mode::opt_od_with_pd_at_middle_speed;
constexpr io_mode opt_od_with_pd_at_fast_speed      = io_mode::opt_od_with_pd_at_fast_speed;
constexpr io_mode opt_od_with_pd_at_high_speed      = io_mode::opt_od_with_pd_at_high_speed;
constexpr io_mode afio_pp_at_low_speed              = io_mode::afio_pp_at_low_speed;
constexpr io_mode afio_pp_at_middle_speed           = io_mode::afio_pp_at_middle_speed;
constexpr io_mode afio_pp_at_fast_speed             = io_mode::afio_pp_at_fast_speed;
constexpr io_mode afio_pp_at_high_speed             = io_mode::afio_pp_at_high_speed;
constexpr io_mode afio_pp_with_pu_at_low_speed      = io_mode::afio_pp_with_pu_at_low_speed;
constexpr io_mode afio_pp_with_pu_at_middle_speed   = io_mode::afio_pp_with_pu_at_middle_speed;
constexpr io_mode afio_pp_with_pu_at_fast_speed     = io_mode::afio_pp_with_pu_at_fast_speed;
constexpr io_mode afio_pp_with_pu_at_high_speed     = io_mode::afio_pp_with_pu_at_high_speed;
constexpr io_mode afio_pp_with_pd_at_low_speed      = io_mode::afio_pp_with_pd_at_low_speed;
constexpr io_mode afio_pp_with_pd_at_middle_speed   = io_mode::afio_pp_with_pd_at_middle_speed;
constexpr io_mode afio_pp_with_pd_at_fast_speed     = io_mode::afio_pp_with_pd_at_fast_speed;
constexpr io_mode afio_pp_with_pd_at_high_speed     = io_mode::afio_pp_with_pd_at_high_speed;
constexpr io_mode afio_od_at_low_speed              = io_mode::afio_od_at_low_speed;
constexpr io_mode afio_od_at_middle_speed           = io_mode::afio_od_at_middle_speed;
constexpr io_mode afio_od_at_fast_speed             = io_mode::afio_od_at_fast_speed;
constexpr io_mode afio_od_at_high_speed             = io_mode::afio_od_at_high_speed;
constexpr io_mode afio_od_with_pu_at_low_speed      = io_mode::afio_od_with_pu_at_low_speed;
constexpr io_mode afio_od_with_pu_at_middle_speed   = io_mode::afio_od_with_pu_at_middle_speed;
constexpr io_mode afio_od_with_pu_at_fast_speed     = io_mode::afio_od_with_pu_at_fast_speed;
constexpr io_mode afio_od_with_pu_at_high_speed     = io_mode::afio_od_with_pu_at_high_speed;
constexpr io_mode afio_od_with_pd_at_low_speed      = io_mode::afio_od_with_pd_at_low_speed;
constexpr io_mode afio_od_with_pd_at_middle_speed   = io_mode::afio_od_with_pd_at_middle_speed;
constexpr io_mode afio_od_with_pd_at_fast_speed     = io_mode::afio_od_with_pd_at_fast_speed;
constexpr io_mode afio_od_with_pd_at_high_speed     = io_mode::afio_od_with_pd_at_high_speed;
constexpr io_mode analog                            = io_mode::analog;

#define xgen(name)                                          \
xstruct(name)                                               \
    template<class ... args>                                \
    name(uxx port, args ... rest){                          \
        implicit_value<uxx> ports[] = { port, rest... };    \
        value = 0;                                          \
        xfor(uxx i = 0; i < 1 + sizeof...(args); i++)       \
            value |= 1 << ports[i];                         \
        $                                                   \
    }                                                       \
    xcvtc(u32)                                              \
        return value;                                       \
    $                                                       \
private:                                                    \
    u32 value;                                              \
$;                                                          

xgen(set);
xgen(reset);
#undef xgen

xunion(_gpio)
private:
    xstruct(mode_t)
        u32 up_down : 2;
        u32 speed   : 2;
        u32 od      : 1;
        u32 mode    : 2;
        xini(mode_t(io_mode value))
            mixc::copy(this, value);
        $
    $;

    xstruct(field)
        u32 mode;
        u32 otype;
        u32 speed;
        u32 pupd;
        u32 idr;
        u32 odr;
        u32 bsr;
    $ items;
public:
    template<class ... args>
    xdefr(_gpio &, config, io_mode mode, uxx port, args ... rest)
        implicit_value<uxx> ports[] = { port, rest... };
        mode_t props = mode;
        u32    modes = items.mode;
        u32    otype = items.otype;
        u32    speed = items.speed;
        u32    pupds = items.pupd;

        xfor(u32 i = 0; i < 1 + sizeof...(args); i++)
            u32 p1 = ports[i];
            u32 p2 = p1 << 1;
            modes &= ~(0x3 << p2);
            speed &= ~(0x3 << p2);
            pupds &= ~(0x3 << p2);
            otype &= ~(0x1 << p1);
            modes |= u32(props.mode) << p2;
            speed |= u32(props.speed) << p2;
            pupds |= u32(props.up_down) << p2;
            otype |= u32(props.od) << p1;
        $

        items.mode  = modes;
        items.otype = otype;
        items.speed = speed;
        items.pupd  = pupds;
        return thex;
    $

    template<class ... args>
    xopr(_gpio &, (), io_mode mode, uxx port, args ... rest)
        return config(mode, port, rest...);
    $

    xunion()
        xcvtc(u32)
            field * ptr = (field *)this;
            return ptr->idr;
        $

        xopc([], uxx index)
            u32 value = thex;
            return (value & (1 << index)) != 0;
        $
    $ ipt;

    xunion(_ipt)
        xcvtc(u32 &)
            field * ptr = (field *)this;
            return ptr->idr;
        $

        xopc([], uxx index)
            field * ptr = (field *)this;
            xstruct(bind_bit)
                xini(bind_bit(u32 & value, u32 index))
                    thex.ptr = & value;
                    thex.bit = 1 << index;
                $

                xcvtc(bool)
                    return (ptr[0] & bit) != 0;
                $

                xop(=, bool value)
                    ptr[0] = value ? ptr[0] | bit : ptr[0] & ~bit;
                $

                u32   bit;
                u32 * ptr;
            $ value { ptr->odr, index };

            return value;
        $

        xdef(parallel, set group_set, reset group_reset)
            field * ptr = (field *)this;
            ptr->bsr = group_reset << 16 | group_set;
        $

        xdef(parallel, set group_set)
            field * ptr = (field *)this;
            u32 group_reset = ptr->bsr & 0xffff0000;
            ptr->bsr = group_reset << 16 | group_set;
        $

        xdef(parallel, reset group_reset)
            field * ptr = (field *)this;
            u32 group_set = ptr->bsr & 0xffff;
            ptr->bsr = group_reset << 16 | group_set;
        $
    $ opt;
$;

#define xirq(name)        void __attribute__((interrupt("irq"))) name() {
#define xseg(name)        __attribute__((section(#name)))
#define xextc             extern "C" {

int main();
xirq(reset_handler) 
    xwhile(true)
        main(); 
    $
$
xirq(nmi_handler) $
xirq(hardfault_handler) $
xirq(memmanage_handler) $
xirq(busfault_handler) $
xirq(usagefault_handler) $
// xirq(0) $
// xirq(0) $
// xirq(0) $
// xirq(0) $
xirq(svc_handler) $
xirq(debugmon_handler) $
// xirq(0) $
xirq(pendsv_handler) $
xirq(systick_handler) $
xirq(wwdg_irqhandler) $
xirq(pvd_irqhandler) $
xirq(tamp_stamp_irqhandler) $
xirq(rtc_wkup_irqhandler) $
xirq(flash_irqhandler) $
xirq(rcc_irqhandler) $
xirq(exti0_irqhandler) $
xirq(exti1_irqhandler) $
xirq(exti2_irqhandler) $
xirq(exti3_irqhandler) $
xirq(exti4_irqhandler) $
xirq(dma1_stream0_irqhandler) $
xirq(dma1_stream1_irqhandler) $
xirq(dma1_stream2_irqhandler) $
xirq(dma1_stream3_irqhandler) $
xirq(dma1_stream4_irqhandler) $
xirq(dma1_stream5_irqhandler) $
xirq(dma1_stream6_irqhandler) $
xirq(adc_irqhandler) $
xirq(can1_tx_irqhandler) $
xirq(can1_rx0_irqhandler) $
xirq(can1_rx1_irqhandler) $
xirq(can1_sce_irqhandler) $
xirq(exti9_5_irqhandler) $
xirq(tim1_brk_tim9_irqhandler) $
xirq(tim1_up_tim10_irqhandler) $
xirq(tim1_trg_com_tim11_irqhandler) $
xirq(tim1_cc_irqhandler) $
xirq(tim2_irqhandler) $
xirq(tim3_irqhandler) $
xirq(tim4_irqhandler) $
xirq(i2c1_ev_irqhandler) $
xirq(i2c1_er_irqhandler) $
xirq(i2c2_ev_irqhandler) $
xirq(i2c2_er_irqhandler) $
xirq(spi1_irqhandler) $
xirq(spi2_irqhandler) $
xirq(usart1_irqhandler) $
xirq(usart2_irqhandler) $
xirq(usart3_irqhandler) $
xirq(exti15_10_irqhandler) $
xirq(rtc_alarm_irqhandler) $
xirq(otg_fs_wkup_irqhandler) $
xirq(tim8_brk_tim12_irqhandler) $
xirq(tim8_up_tim13_irqhandler) $
xirq(tim8_trg_com_tim14_irqhandler) $
xirq(tim8_cc_irqhandler) $
xirq(dma1_stream7_irqhandler) $
xirq(fmc_irqhandler) $
xirq(sdmmc1_irqhandler) $
xirq(tim5_irqhandler) $
xirq(spi3_irqhandler) $
xirq(uart4_irqhandler) $
xirq(uart5_irqhandler) $
xirq(tim6_dac_irqhandler) $
xirq(tim7_irqhandler) $
xirq(dma2_stream0_irqhandler) $
xirq(dma2_stream1_irqhandler) $
xirq(dma2_stream2_irqhandler) $
xirq(dma2_stream3_irqhandler) $
xirq(dma2_stream4_irqhandler) $
xirq(eth_irqhandler) $
xirq(eth_wkup_irqhandler) $
xirq(can2_tx_irqhandler) $
xirq(can2_rx0_irqhandler) $
xirq(can2_rx1_irqhandler) $
xirq(can2_sce_irqhandler) $
xirq(otg_fs_irqhandler) $
xirq(dma2_stream5_irqhandler) $
xirq(dma2_stream6_irqhandler) $
xirq(dma2_stream7_irqhandler) $
xirq(usart6_irqhandler) $
xirq(i2c3_ev_irqhandler) $
xirq(i2c3_er_irqhandler) $
xirq(otg_hs_ep1_out_irqhandler) $
xirq(otg_hs_ep1_in_irqhandler) $
xirq(otg_hs_wkup_irqhandler) $
xirq(otg_hs_irqhandler) $
xirq(dcmi_irqhandler) $
// xirq(0) $
xirq(rng_irqhandler) $
xirq(fpu_irqhandler) $
xirq(uart7_irqhandler) $
xirq(uart8_irqhandler) $
xirq(spi4_irqhandler) $
xirq(spi5_irqhandler) $
xirq(spi6_irqhandler) $
xirq(sai1_irqhandler) $
// xirq(0) $
// xirq(0) $
xirq(dma2d_irqhandler) $
xirq(sai2_irqhandler) $
xirq(quadspi_irqhandler) $
xirq(lptim1_irqhandler) $
xirq(cec_irqhandler) $
xirq(i2c4_ev_irqhandler) $
xirq(i2c4_er_irqhandler) $
xirq(spdif_rx_irqhandler) $

extern int __stack_top;
typedef void(* irq_t)();

xseg(.irqs) irq_t irq_vector[] = {
    irq_t(& __stack_top),
    reset_handler,
    nmi_handler,
    hardfault_handler,
    memmanage_handler,
    busfault_handler,
    usagefault_handler,
    0,
    0,
    0,
    0,
    svc_handler,
    debugmon_handler,
    0,
    pendsv_handler,
    systick_handler,
    
    /* external interrupts */
    wwdg_irqhandler,                   /* window watchdog              */                                        
    pvd_irqhandler,                    /* pvd through exti line detection */                        
    tamp_stamp_irqhandler,             /* tamper and timestamps through the exti line */            
    rtc_wkup_irqhandler,               /* rtc wakeup through the exti line */                      
    flash_irqhandler,                  /* flash                        */                                          
    rcc_irqhandler,                    /* rcc                          */                                            
    exti0_irqhandler,                  /* exti line0                   */                        
    exti1_irqhandler,                  /* exti line1                   */                          
    exti2_irqhandler,                  /* exti line2                   */                          
    exti3_irqhandler,                  /* exti line3                   */                          
    exti4_irqhandler,                  /* exti line4                   */                          
    dma1_stream0_irqhandler,           /* dma1 stream 0                */                  
    dma1_stream1_irqhandler,           /* dma1 stream 1                */                   
    dma1_stream2_irqhandler,           /* dma1 stream 2                */                   
    dma1_stream3_irqhandler,           /* dma1 stream 3                */                   
    dma1_stream4_irqhandler,           /* dma1 stream 4                */                   
    dma1_stream5_irqhandler,           /* dma1 stream 5                */                   
    dma1_stream6_irqhandler,           /* dma1 stream 6                */                   
    adc_irqhandler,                    /* adc1, adc2 and adc3s         */                   
    can1_tx_irqhandler,                /* can1 tx                      */                         
    can1_rx0_irqhandler,               /* can1 rx0                     */                          
    can1_rx1_irqhandler,               /* can1 rx1                     */                          
    can1_sce_irqhandler,               /* can1 sce                     */                          
    exti9_5_irqhandler,                /* external line[9:5]s          */                          
    tim1_brk_tim9_irqhandler,          /* tim1 break and tim9          */         
    tim1_up_tim10_irqhandler,          /* tim1 update and tim10        */         
    tim1_trg_com_tim11_irqhandler,     /* tim1 trigger and commutation and tim11 */
    tim1_cc_irqhandler,                /* tim1 capture compare         */                          
    tim2_irqhandler,                   /* tim2                         */                   
    tim3_irqhandler,                   /* tim3                         */                   
    tim4_irqhandler,                   /* tim4                         */                   
    i2c1_ev_irqhandler,                /* i2c1 event                   */                          
    i2c1_er_irqhandler,                /* i2c1 error                   */                          
    i2c2_ev_irqhandler,                /* i2c2 event                   */                          
    i2c2_er_irqhandler,                /* i2c2 error                   */                            
    spi1_irqhandler,                   /* spi1                         */                   
    spi2_irqhandler,                   /* spi2                         */                   
    usart1_irqhandler,                 /* usart1                       */                   
    usart2_irqhandler,                 /* usart2                       */                   
    usart3_irqhandler,                 /* usart3                       */                   
    exti15_10_irqhandler,              /* external line[15:10]s        */                          
    rtc_alarm_irqhandler,              /* rtc alarm (a and b) through exti line */                 
    otg_fs_wkup_irqhandler,            /* usb otg fs wakeup through exti line */                       
    tim8_brk_tim12_irqhandler,         /* tim8 break and tim12         */         
    tim8_up_tim13_irqhandler,          /* tim8 update and tim13        */         
    tim8_trg_com_tim14_irqhandler,     /* tim8 trigger and commutation and tim14 */
    tim8_cc_irqhandler,                /* tim8 capture compare         */                          
    dma1_stream7_irqhandler,           /* dma1 stream7                 */                          
    fmc_irqhandler,                    /* fmc                          */                   
    sdmmc1_irqhandler,                 /* sdmmc1                       */                   
    tim5_irqhandler,                   /* tim5                         */                   
    spi3_irqhandler,                   /* spi3                         */                   
    uart4_irqhandler,                  /* uart4                        */                   
    uart5_irqhandler,                  /* uart5                        */                   
    tim6_dac_irqhandler,               /* tim6 and dac1&2 underrun errors */                   
    tim7_irqhandler,                   /* tim7                         */
    dma2_stream0_irqhandler,           /* dma2 stream 0                */                   
    dma2_stream1_irqhandler,           /* dma2 stream 1                */                   
    dma2_stream2_irqhandler,           /* dma2 stream 2                */                   
    dma2_stream3_irqhandler,           /* dma2 stream 3                */                   
    dma2_stream4_irqhandler,           /* dma2 stream 4                */                   
    eth_irqhandler,                    /* ethernet                     */                   
    eth_wkup_irqhandler,               /* ethernet wakeup through exti line */                     
    can2_tx_irqhandler,                /* can2 tx                      */                          
    can2_rx0_irqhandler,               /* can2 rx0                     */                          
    can2_rx1_irqhandler,               /* can2 rx1                     */                          
    can2_sce_irqhandler,               /* can2 sce                     */                          
    otg_fs_irqhandler,                 /* usb otg fs                   */                   
    dma2_stream5_irqhandler,           /* dma2 stream 5                */                   
    dma2_stream6_irqhandler,           /* dma2 stream 6                */                   
    dma2_stream7_irqhandler,           /* dma2 stream 7                */                   
    usart6_irqhandler,                 /* usart6                       */                    
    i2c3_ev_irqhandler,                /* i2c3 event                   */                          
    i2c3_er_irqhandler,                /* i2c3 error                   */                          
    otg_hs_ep1_out_irqhandler,         /* usb otg hs end point 1 out   */                   
    otg_hs_ep1_in_irqhandler,          /* usb otg hs end point 1 in    */                   
    otg_hs_wkup_irqhandler,            /* usb otg hs wakeup through exti */                         
    otg_hs_irqhandler,                 /* usb otg hs                   */                   
    dcmi_irqhandler,                   /* dcmi                         */                   
    0,                                 /* reserved                     */                   
    rng_irqhandler,                    /* rng                          */
    fpu_irqhandler,                    /* fpu                          */
    uart7_irqhandler,                  /* uart7                        */      
    uart8_irqhandler,                  /* uart8                        */
    spi4_irqhandler,                   /* spi4                         */
    spi5_irqhandler,                   /* spi5	 		              */
    spi6_irqhandler,                   /* spi6   			          */
    sai1_irqhandler,                   /* sai1						  */
    0,                                 /* reserved                     */
    0,                                 /* reserved                     */
    dma2d_irqhandler,                  /* dma2d    				      */
    sai2_irqhandler,                   /* sai2                         */
    quadspi_irqhandler,                /* quadspi                      */
    lptim1_irqhandler,                 /* lptim1                       */
    cec_irqhandler,                    /* hdmi_cec                     */
    i2c4_ev_irqhandler,                /* i2c4 event                   */
    i2c4_er_irqhandler,                /* i2c4 error                   */
    spdif_rx_irqhandler,               /* spdif_rx                     */  
};


#define rcc     (*(_rcc * )0x40023800)
#define ioa     (*(_gpio *)0x40020000)
#define iob     (*(_gpio *)0x40020400)
#define ioc     (*(_gpio *)0x40020800)
#define iod     (*(_gpio *)0x40020c00)
#define ioe     (*(_gpio *)0x40021000)
#define iof     (*(_gpio *)0x40021400)
#define iog     (*(_gpio *)0x40021800)
#define ioh     (*(_gpio *)0x40021c00)
#define ioi     (*(_gpio *)0x40022000)
#define ioj     (*(_gpio *)0x40022400)
#define iok     (*(_gpio *)0x40022800)

