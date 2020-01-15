#pragma once
#include"definition/mini.hpp"

#define xro(type,name,target)                   \
    union {                                     \
        operator type(){                        \
            field * ptr = (field *)this;        \
            return type((u32)ptr->target);      \
        }                                       \
    } name;

#define xrw(type,name,target)                   \
    union {                                     \
        operator type(){                        \
            field * ptr = (field *)this;        \
            return type((u32)ptr->target);      \
        }                                       \
        void operator =(type value){            \
            field * ptr = (field *)this;        \
            ptr->target = u32(value);           \
        }                                       \
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

    xunion()
        xrw(u32, period, modper);
        xrw(u32, step, incstep);
        xrw(spread_mode, mode, spreadsel);
        xrw(bool, on, sscgen);
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
        xrw(bool, on, lseon);
        xro(bool, ready, lserdy);
        xrw(bool, bypass, lsebyp);
        xrw(::drive_capability, drive_capability, lsedrv);
    $ lse;

    xunion()
        xrw(bool, on, lsion);
        xro(bool, ready, lsirdy);
    $ lsi;

    xunion()
        xrw(bool, on, hsion);
        xro(bool, ready, hsirdy);
        xrw(u32 , triming, hsitrim);
        xro(u32 , calibration, hsical);
    $ hsi;

    xunion()
        xrw(bool, on, hseon);
        xrw(bool, ready, hserdy);
        xrw(bool, bypass, hsebyp);
        xrw(bool, safe_clock, csson);

        // divsion from hse
        // rtc clock need equals to 1MHz
        xrw(::for_rtc, for_rtc, rtcpre);
    $ hse;

    xunion()
        xrw(bool, on, plli2son);
        xrw(bool, ready, plli2srdy);
        xrw(source_for_i2s, source, i2ssrc);
    $ i2s;

    xunion()
        xrw(bool, on, pllsaion);
        xrw(bool, ready, pllsairdy);
    $ sai;

    xunion()
        xrw(bool, on, pllon);
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
        xrw(bool, on, rtcen);
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


