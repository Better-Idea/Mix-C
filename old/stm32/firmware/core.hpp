#pragma once
#include"definition/mini.hpp"
#define xvref         3.3
#define xflush        asm("":::"memory");
#define xfield        (*(field *)this)
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
    void operator()(a a0, b a1, args... rest) {\
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
    xassume(source_for_rtc , 1);
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
    name(uxx port, args... rest){                          \
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
    xunion(_config)
        template<class ... args>
        xini(_config(io_mode mode, uxx port, args... rest))
            implicit_value<uxx> ports[] = { port, rest... };
            mode_t props = mode;
            u32    modes = xfield.mode;
            u32    otype = xfield.otype;
            u32    speed = xfield.speed;
            u32    pupds = xfield.pupd;

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

            xfield.mode  = modes;
            xfield.otype = otype;
            xfield.speed = speed;
            xfield.pupd  = pupds;
        $
    $ config;


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

    xunion()
        xcvtc(u32 &)
            return xfield.odr;
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
            $ value { thex, index };

            return value;
        $

        xunion(_parallel)
            xini(_parallel(set group_set))
                u32 group_reset = xfield.bsr & 0xffff0000;
                xfield.bsr = group_reset << 16 | group_set;
            $

            xini(_parallel(reset group_reset))
                u32 group_set = xfield.bsr & 0xffff;
                xfield.bsr = group_reset << 16 | group_set;
            $

            xini(_parallel(set group_set, reset group_reset))
                xfield.bsr = group_reset << 16 | group_set;
            $
        $ parallel;
    $ opt;
$;



xenumc(source_for_channel_trigger)
    from_tim6,
    from_tim8,
    from_tim7,
    from_tim5,
    from_tim2,
    from_tim4,
    from_exti_line9,
    from_software,
$;

xenumc(ware_type)
    none_ware,
    noise_ware,
    triangle_ware,
$;

xenumc(triangle_ware_amplitude)
    equals_to_1, equals_to_3, equals_to_7, equals_to_15, equals_to_31, equals_to_63, equals_to_127, equals_to_255, equals_to_511, equals_to_1023, equals_to_2047, equals_to_4095,
$;

xenumc(noise_ware_range)
    less_than_2, less_than_4, less_than_8, less_than_16, less_than_32, less_than_64, less_than_128, less_than_256, less_than_512, less_than_1024, less_than_2048, less_than_4096,
$;

xstruct(not_t)
    xini(not_t(bool value = false) : value(value)) $
    xcvt(bool)
        return !value;
    $
    xop(=, bool value)
        thex.value = !value;
    $
private:
    bool value;
$;

constexpr source_for_channel_trigger from_tim6          = source_for_channel_trigger::from_tim6;
constexpr source_for_channel_trigger from_tim8          = source_for_channel_trigger::from_tim8;
constexpr source_for_channel_trigger from_tim7          = source_for_channel_trigger::from_tim7;
constexpr source_for_channel_trigger from_tim5          = source_for_channel_trigger::from_tim5;
constexpr source_for_channel_trigger from_tim2          = source_for_channel_trigger::from_tim2;
constexpr source_for_channel_trigger from_tim4          = source_for_channel_trigger::from_tim4;
constexpr source_for_channel_trigger from_exti_line9    = source_for_channel_trigger::from_exti_line9;
constexpr source_for_channel_trigger from_software      = source_for_channel_trigger::from_software;
constexpr ware_type                  none_ware          = ware_type::none_ware;
constexpr ware_type                  noise_ware         = ware_type::noise_ware;
constexpr ware_type                  triangle_ware      = ware_type::triangle_ware;
constexpr triangle_ware_amplitude    equals_to_1        = triangle_ware_amplitude::equals_to_1;
constexpr triangle_ware_amplitude    equals_to_3        = triangle_ware_amplitude::equals_to_3;
constexpr triangle_ware_amplitude    equals_to_7        = triangle_ware_amplitude::equals_to_7;
constexpr triangle_ware_amplitude    equals_to_15       = triangle_ware_amplitude::equals_to_15;
constexpr triangle_ware_amplitude    equals_to_31       = triangle_ware_amplitude::equals_to_31;
constexpr triangle_ware_amplitude    equals_to_63       = triangle_ware_amplitude::equals_to_63;
constexpr triangle_ware_amplitude    equals_to_127      = triangle_ware_amplitude::equals_to_127;
constexpr triangle_ware_amplitude    equals_to_255      = triangle_ware_amplitude::equals_to_255;
constexpr triangle_ware_amplitude    equals_to_511      = triangle_ware_amplitude::equals_to_511;
constexpr triangle_ware_amplitude    equals_to_1023     = triangle_ware_amplitude::equals_to_1023;
constexpr triangle_ware_amplitude    equals_to_2047     = triangle_ware_amplitude::equals_to_2047;
constexpr triangle_ware_amplitude    equals_to_4095     = triangle_ware_amplitude::equals_to_4095;
constexpr noise_ware_range           less_than_2        = noise_ware_range::less_than_2;
constexpr noise_ware_range           less_than_4        = noise_ware_range::less_than_4;
constexpr noise_ware_range           less_than_8        = noise_ware_range::less_than_8;
constexpr noise_ware_range           less_than_16       = noise_ware_range::less_than_16;
constexpr noise_ware_range           less_than_32       = noise_ware_range::less_than_32;
constexpr noise_ware_range           less_than_64       = noise_ware_range::less_than_64;
constexpr noise_ware_range           less_than_128      = noise_ware_range::less_than_128;
constexpr noise_ware_range           less_than_256      = noise_ware_range::less_than_256;
constexpr noise_ware_range           less_than_512      = noise_ware_range::less_than_512;
constexpr noise_ware_range           less_than_1024     = noise_ware_range::less_than_1024;
constexpr noise_ware_range           less_than_2048     = noise_ware_range::less_than_2048;
constexpr noise_ware_range           less_than_4096     = noise_ware_range::less_than_4096;

// NOTE:
// - dac1 & dac2 combine operation register is leave out
// - 8bit precision is leave out
// - left align mode is leave out
xunion(_dac)
private:
    xstruct(field)
        u32 en1         : 1;
        u32 boff1       : 1;
        u32 ten1        : 1;
        u32 tsel1       : 3;
        u32 ware1       : 2;
        u32 mamp1       : 4;
        u32 dmaen1      : 1;
        u32 dmaudrie1   : 1;
        xr32(2);
        u32 en2         : 1;
        u32 boff2       : 1;
        u32 ten2        : 1;
        u32 tsel2       : 3;
        u32 ware2       : 2;
        u32 mamp2       : 4;
        u32 dmaen2      : 1;
        u32 dmaudrie2   : 1;
        xr32(2);

        u32 swtrig1     : 1;
        u32 swtrig2     : 1;
        xr32(30);

        u32 dhr12r1     : 12;
        xr32(20);

        xr32(4);
        u32 dhr12l1     : 12;
        xr32(16);

        u32 dhr8r1      : 8;
        xr32(24);

        u32 dhr12r2     : 12;
        xr32(20);

        xr32(4);
        u32 dhr12l2     : 12;
        xr32(16);

        u32 dhr8r2      : 8;
        xr32(24);

        u32 dhr12rd1    : 12;
        xr32(4);
        u32 dhr12rd2    : 12;
        xr32(4);

        xr32(4);
        u32 dhr12ld1    : 12;
        xr32(4);
        u32 dhr12ld2    : 12;
        
        u32 hdr8rd1     : 8;
        u32 hdr8rd2     : 8;
        xr32(16);

        u32 dor1        : 12;
        xr32(20);

        u32 dor2        : 12;
        xr32(20);

        xr32(13);
        u32 dmaudr1     : 1;
        xr32(2);
        xr32(13);
        u32 dmaudr2     : 1;
        xr32(2);
    $ items;

public:
    xunion()
        /* - 0 : diable dac channel */
        /* - 1 : enable dac channel */
        xrw(bool, enable, en1);

        /* - 0 : disable dac output buffer */
        /* - 1 : enable dac output buffer */
        xrw(not_t, output_buffer, boff1);

        /* - 0 : wrtie data to data hold register whill move to data output register after 1 apb1 clock */
        /* - 1 : wrtie data to data hold register whill move to data output register after 3 apb1 clock */
        xunion(_triggle)
            xini(_triggle(bool enable = false))
                xfield.ten1  = enable;
            $

            xini(_triggle(source_for_channel_trigger source))
                xfield.tsel1 = u32(source);
                xfield.ten1  = u32(1);
            $

            xini(_triggle(source_for_channel_trigger source, triangle_ware_amplitude amplitude))
                xfield.mamp1 = u32(amplitude);
                xfield.ware1 = u32(2); /* triangle_ware  */
                xfield.tsel1 = u32(source);
                xfield.ten1  = u32(1);
            $

            xini(_triggle(source_for_channel_trigger source, noise_ware_range range))
                xfield.mamp1 = u32(range);
                xfield.ware1 = u32(1); /* noise_ware  */
                xfield.tsel1 = u32(source);
                xfield.ten1  = u32(1);
            $

            xrw(bool, enable, en1);
            xrw(source_for_channel_trigger, source, ten1);
            xrw(ware_type, type, ware1);
            xrw(::triangle_ware_amplitude, triangle_ware_amplitude, mamp1);
            xrw(::noise_ware_range, noise_ware_range, mamp1);
        $ triggle;

        xunion()
            xrw(bool, enable, dmaen1);
            xrw(bool, underrun_interrupt, dmaudrie1);

            xunion()
                xcvt(bool)
                    return xfield.dmaudr1 == 1;
                $
                xop(=, clear_t)
                    xfield.dmaudr1 = 1; /* write 1 to clear */
                $
            $ underrun;
        $ dma;

        /* this bit is clear by hardware(one APB1 clock cycle later)
           once the data hold register value has been loaded into data output register */
        xrw(bool, software_triggler, swtrig1);

        /* 12bit precision */
        xunion()
            xop(=, float value)
                mf32 ref = xvref;
                mf32 val = value;
                u32    r = ref.decimal | 1 << 23;
                u32    v = val.decimal | 1 << 23;
                r      >>= (ref.real_exp - val.real_exp) + 4; // 4 + 8 -> 12bit
                v      <<= 8;
                v       /= r;
                xfield.dhr12r1 = v;
            $
            xcvt(u32)
                return u32(xfield.dhr12r1);
            $
        $ voltage;

        xunion()
            xcvt(float)
                return (xfield.dor1);
            $
        $ opt;
    $ one;
$;




xenumc(latch_when)
    falling_edge,
    rising_edge,
$;

xenumc(when_spi_idle)
    clock_stay_low_level,
    clock_stay_high_level,
$;

xenumc(spi_role)
    as_slaver,
    as_master,
$;

xenumc(spi_bits_order)
    msb_first,
    lsb_first,
$;

xenumc(spi_baud_rate)
    from_apb_div2, from_apb_div4, from_apb_div8, from_apb_div16, from_apb_div32, from_apb_div64, from_apb_div128, from_apb_div256,
$;

xunion(_spi)
private:
    xstruct(field)
        u32 cpha        : 1;
        u32 cpol        : 1;
        u32 mstr        : 1;
        u32 br          : 3;
        u32 spe         : 1;
        u32 lsbfirst    : 1;
        u32 ssi         : 1;
        u32 ssm         : 1;
        u32 rxonly      : 1;
        u32 crcl        : 1;
        u32 crcnext     : 1;
        u32 crcen       : 1;
        u32 bidioe      : 1;
        u32 bidimode    : 1;
        xr32(16);

        u32 rxdmaen     : 1;
        u32 txdmaen     : 1;
        u32 ssoe        : 1;
        u32 nssp        : 1;
        u32 frf         : 1;
        u32 errie       : 1;
        u32 rxneie      : 1;
        u32 txeie       : 1;
        u32 ds          : 4;
        u32 frxth       : 1;
        u32 ldma_rx     : 1;
        u32 ldma_tx     : 1;
        xr32(17);
        u32 : 1;
        u32 : 1;
        u32 : 1;
        u32 : 1;
        u32 : 1;
        u32 : 1;
        u32 : 1;
        u32 : 1;
    $ items;
public:
    xrw(::latch_when, latch_when, cpha);
    xrw(::when_spi_idle, when_idle, cpol);
    xrw(::spi_role, role, mstr);
    xrw(::spi_baud_rate, source, br);
    xrw(bool, enable, spe);
    xrw(::spi_bits_order, bits_order, lsbfirst);
$;

#define xirq(name)        void __attribute__((interrupt("irq"))) name() {
#define xseg(name)        __attribute__((section(#name)))
#define xextc             extern "C" {

xirq(itr_reset) 
    int main();
    xwhile(true)
        main(); 
    $
$
xirq(itr_nmi) $
xirq(itr_hardfault) $
xirq(itr_memmanage) $
xirq(itr_busfault) $
xirq(itr_usagefault) $
// xirq(0) $
// xirq(0) $
// xirq(0) $
// xirq(0) $
xirq(itr_svc) $
xirq(itr_debugmon) $
// xirq(0) $
xirq(itr_pendsv) $
xirq(itr_systick) $
xirq(irq_wwdg) $
xirq(irq_pvd) $
xirq(irq_tamp_stamp) $
xirq(irq_rtc_wkup) $
xirq(irq_flash) $
xirq(irq_rcc) $
xirq(irq_exti0) $
xirq(irq_exti1) $
xirq(irq_exti2) $
xirq(irq_exti3) $
xirq(irq_exti4) $
xirq(irq_dma1_stream0) $
xirq(irq_dma1_stream1) $
xirq(irq_dma1_stream2) $
xirq(irq_dma1_stream3) $
xirq(irq_dma1_stream4) $
xirq(irq_dma1_stream5) $
xirq(irq_dma1_stream6) $
xirq(irq_adc) $
xirq(irq_can1_tx) $
xirq(irq_can1_rx0) $
xirq(irq_can1_rx1) $
xirq(irq_can1_sce) $
xirq(irq_exti9_5) $
xirq(irq_tim1_brk_tim9) $
xirq(irq_tim1_up_tim10) $
xirq(irq_tim1_trg_com_tim11) $
xirq(irq_tim1_cc) $
xirq(irq_tim2) $
xirq(irq_tim3) $
xirq(irq_tim4) $
xirq(irq_i2c1_ev) $
xirq(irq_i2c1_er) $
xirq(irq_i2c2_ev) $
xirq(irq_i2c2_er) $
xirq(irq_spi1) $
xirq(irq_spi2) $
xirq(irq_usart1) $
xirq(irq_usart2) $
xirq(irq_usart3) $
xirq(irq_exti15_10) $
xirq(irq_rtc_alarm) $
xirq(irq_otg_fs_wkup) $
xirq(irq_tim8_brk_tim12) $
xirq(irq_tim8_up_tim13) $
xirq(irq_tim8_trg_com_tim14) $
xirq(irq_tim8_cc) $
xirq(irq_dma1_stream7) $
xirq(irq_fmc) $
xirq(irq_sdmmc1) $
xirq(irq_tim5) $
xirq(irq_spi3) $
xirq(irq_uart4) $
xirq(irq_uart5) $
xirq(irq_tim6_dac) $
xirq(irq_tim7) $
xirq(irq_dma2_stream0) $
xirq(irq_dma2_stream1) $
xirq(irq_dma2_stream2) $
xirq(irq_dma2_stream3) $
xirq(irq_dma2_stream4) $
xirq(irq_eth) $
xirq(irq_eth_wkup) $
xirq(irq_can2_tx) $
xirq(irq_can2_rx0) $
xirq(irq_can2_rx1) $
xirq(irq_can2_sce) $
xirq(irq_otg_fs) $
xirq(irq_dma2_stream5) $
xirq(irq_dma2_stream6) $
xirq(irq_dma2_stream7) $
xirq(irq_usart6) $
xirq(irq_i2c3_ev) $
xirq(irq_i2c3_er) $
xirq(irq_otg_hs_ep1_out) $
xirq(irq_otg_hs_ep1_in) $
xirq(irq_otg_hs_wkup) $
xirq(irq_otg_hs) $
xirq(irq_dcmi) $
// xirq(0) $
xirq(irq_rng) $
xirq(irq_fpu) $
xirq(irq_uart7) $
xirq(irq_uart8) $
xirq(irq_spi4) $
xirq(irq_spi5) $
xirq(irq_spi6) $
xirq(irq_sai1) $
// xirq(0) $
// xirq(0) $
xirq(irq_dma2d) $
xirq(irq_sai2) $
xirq(irq_quadspi) $
xirq(irq_lptim1) $
xirq(irq_cec) $
xirq(irq_i2c4_ev) $
xirq(irq_i2c4_er) $
xirq(irq_spdif_rx) $

extern int __stack_top;
typedef void(* irq_t)();

xseg(.irqs) irq_t irq_vector[] = {
    irq_t(& __stack_top),
    itr_reset,
    itr_nmi,
    itr_hardfault,
    itr_memmanage,
    itr_busfault,
    itr_usagefault,
    0,
    0,
    0,
    0,
    itr_svc,
    itr_debugmon,
    0,
    itr_pendsv,
    itr_systick,
    
    /* external interrupts */
    irq_wwdg,                   /* window watchdog                               */
    irq_pvd,                    /* pvd through exti line detection               */
    irq_tamp_stamp,             /* tamper and timestamps through the exti line   */
    irq_rtc_wkup,               /* rtc wakeup through the exti line              */
    irq_flash,                  /* flash                                         */
    irq_rcc,                    /* rcc                                           */
    irq_exti0,                  /* exti line0                                    */
    irq_exti1,                  /* exti line1                                    */
    irq_exti2,                  /* exti line2                                    */
    irq_exti3,                  /* exti line3                                    */
    irq_exti4,                  /* exti line4                                    */
    irq_dma1_stream0,           /* dma1 stream 0                                 */
    irq_dma1_stream1,           /* dma1 stream 1                                 */
    irq_dma1_stream2,           /* dma1 stream 2                                 */
    irq_dma1_stream3,           /* dma1 stream 3                                 */
    irq_dma1_stream4,           /* dma1 stream 4                                 */
    irq_dma1_stream5,           /* dma1 stream 5                                 */
    irq_dma1_stream6,           /* dma1 stream 6                                 */
    irq_adc,                    /* adc1, adc2 and adc3s                          */
    irq_can1_tx,                /* can1 tx                                       */
    irq_can1_rx0,               /* can1 rx0                                      */
    irq_can1_rx1,               /* can1 rx1                                      */
    irq_can1_sce,               /* can1 sce                                      */
    irq_exti9_5,                /* external line[9:5]s                           */
    irq_tim1_brk_tim9,          /* tim1 break and tim9                           */
    irq_tim1_up_tim10,          /* tim1 update and tim10                         */
    irq_tim1_trg_com_tim11,     /* tim1 trigger and commutation and tim11        */
    irq_tim1_cc,                /* tim1 capture compare                          */
    irq_tim2,                   /* tim2                                          */
    irq_tim3,                   /* tim3                                          */
    irq_tim4,                   /* tim4                                          */
    irq_i2c1_ev,                /* i2c1 event                                    */
    irq_i2c1_er,                /* i2c1 error                                    */
    irq_i2c2_ev,                /* i2c2 event                                    */
    irq_i2c2_er,                /* i2c2 error                                    */
    irq_spi1,                   /* spi1                                          */
    irq_spi2,                   /* spi2                                          */
    irq_usart1,                 /* usart1                                        */
    irq_usart2,                 /* usart2                                        */
    irq_usart3,                 /* usart3                                        */
    irq_exti15_10,              /* external line[15:10]s                         */
    irq_rtc_alarm,              /* rtc alarm (a and b) through exti line         */
    irq_otg_fs_wkup,            /* usb otg fs wakeup through exti line           */
    irq_tim8_brk_tim12,         /* tim8 break and tim12                          */
    irq_tim8_up_tim13,          /* tim8 update and tim13                         */
    irq_tim8_trg_com_tim14,     /* tim8 trigger and commutation and tim14        */
    irq_tim8_cc,                /* tim8 capture compare                          */
    irq_dma1_stream7,           /* dma1 stream7                                  */
    irq_fmc,                    /* fmc                                           */
    irq_sdmmc1,                 /* sdmmc1                                        */
    irq_tim5,                   /* tim5                                          */
    irq_spi3,                   /* spi3                                          */
    irq_uart4,                  /* uart4                                         */
    irq_uart5,                  /* uart5                                         */
    irq_tim6_dac,               /* tim6 and dac1&2 underrun errors               */
    irq_tim7,                   /* tim7                                          */
    irq_dma2_stream0,           /* dma2 stream 0                                 */
    irq_dma2_stream1,           /* dma2 stream 1                                 */
    irq_dma2_stream2,           /* dma2 stream 2                                 */
    irq_dma2_stream3,           /* dma2 stream 3                                 */
    irq_dma2_stream4,           /* dma2 stream 4                                 */
    irq_eth,                    /* ethernet                                      */
    irq_eth_wkup,               /* ethernet wakeup through exti line             */
    irq_can2_tx,                /* can2 tx                                       */
    irq_can2_rx0,               /* can2 rx0                                      */
    irq_can2_rx1,               /* can2 rx1                                      */
    irq_can2_sce,               /* can2 sce                                      */
    irq_otg_fs,                 /* usb otg fs                                    */
    irq_dma2_stream5,           /* dma2 stream 5                                 */
    irq_dma2_stream6,           /* dma2 stream 6                                 */
    irq_dma2_stream7,           /* dma2 stream 7                                 */
    irq_usart6,                 /* usart6                                        */
    irq_i2c3_ev,                /* i2c3 event                                    */
    irq_i2c3_er,                /* i2c3 error                                    */
    irq_otg_hs_ep1_out,         /* usb otg hs end point 1 out                    */
    irq_otg_hs_ep1_in,          /* usb otg hs end point 1 in                     */
    irq_otg_hs_wkup,            /* usb otg hs wakeup through exti                */
    irq_otg_hs,                 /* usb otg hs                                    */
    irq_dcmi,                   /* dcmi                                          */
    0,                          /* reserved                                      */
    irq_rng,                    /* rng                                           */
    irq_fpu,                    /* fpu                                           */
    irq_uart7,                  /* uart7                                         */
    irq_uart8,                  /* uart8                                         */
    irq_spi4,                   /* spi4                                          */
    irq_spi5,                   /* spi5                                          */
    irq_spi6,                   /* spi6                                          */
    irq_sai1,                   /* sai1                                          */
    0,                          /* reserved                                      */
    0,                          /* reserved                                      */
    irq_dma2d,                  /* dma2d                                         */
    irq_sai2,                   /* sai2                                          */
    irq_quadspi,                /* quadspi                                       */
    irq_lptim1,                 /* lptim1                                        */
    irq_cec,                    /* hdmi_cec                                      */
    irq_i2c4_ev,                /* i2c4 event                                    */
    irq_i2c4_er,                /* i2c4 error                                    */
    irq_spdif_rx,               /* spdif_rx                                      */
};

xspace()
    auto & rcc      = (*(_rcc * )0x40023800);
    auto & ioa      = (*(_gpio *)0x40020000);
    auto & iob      = (*(_gpio *)0x40020400);
    auto & ioc      = (*(_gpio *)0x40020800);
    auto & iod      = (*(_gpio *)0x40020c00);
    auto & ioe      = (*(_gpio *)0x40021000);
    auto & iof      = (*(_gpio *)0x40021400);
    auto & iog      = (*(_gpio *)0x40021800);
    auto & ioh      = (*(_gpio *)0x40021c00);
    auto & ioi      = (*(_gpio *)0x40022000);
    auto & ioj      = (*(_gpio *)0x40022400);
    auto & iok      = (*(_gpio *)0x40022800);
    auto & dac      = (*(_dac  *)0x40007400);
$

