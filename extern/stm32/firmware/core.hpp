#pragma once
#include"definition/mini.hpp"

#define xro(type,name,target)                   \
    union _ ## name{                            \
        operator type(){                        \
            field * ptr = (field *)this;        \
            return type((u32)ptr->target);      \
        }                                       \
    }name

// read/clear/enable
#define xevent(name,flag,clear,enable)          \
    union _ ## name{                            \
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

#define xrw(type,name,target)                   \
    union _ ## name{                            \
        operator type(){                        \
            field * ptr = (field *)this;        \
            return type((u32)ptr->target);      \
        }                                       \
        void operator=(type value){             \
            field * ptr = (field *)this;        \
            ptr->target = u32(value);           \
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

#define xport(name)                             \
    union _ ## name{                            \
        xrwpair(port, xlink(name, rst));        \
        xrwpair(bool, xlink(name, en));         \
        xrwpair(low_power_mode,                 \
            xlink(name, lpen));                 \
        xgroup_set()                            \
    } name;

#define xport_en_lpen(name)                     \
    union _ ## name{                            \
        xrwpair(port, xlink(name, en));         \
        xrwpair(low_power_mode,                 \
            xlink(name, lpen));                 \
        xgroup_set()                            \
    } name;

#define xport_lpen(name)                        \
    union _ ## name{                            \
        xrwpair(low_power_mode,                 \
            xlink(name, lpen));                 \
    } name;

#define xport_rst(name)                         \
    union _ ## name{                            \
        xrwpair(port,                           \
            xlink(name, rst));                  \
    } name;

#define xassume(type,value)                     \
    xenumc(type) $;                             \
    xcvtc(type)                                 \
        return type(value);                     \
    $

#define xassumex(type,value)                    \
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
    close_clock,
    keep_clock,
$;

constexpr clear_t clear {};
constexpr bool    enable  = true;
constexpr bool    disable = false;

xenumc(div_for_sclk)
    from_vco_div2, from_vco_div4, from_vco_div6, from_vco_div8,
$;

xenumc(div_for_otg_sdio_random)
    from_vco_div2 = 2,
    from_vco_div3, from_vco_div4, from_vco_div5, from_vco_div6, from_vco_div7, from_vco_div8, from_vco_div9, from_vco_div10, from_vco_div11, from_vco_div12, from_vco_div13, from_vco_div14, from_vco_div15,
$;

xstruct(division)
    division(u32 value) : value(value){}
    xassume(for_pllm, value);
    xassume(for_pllp, value / 2 - 1);
    xassume(for_otg_sdio_random, value);
private:
    u32 value;
$;

xstruct(multiplier)
    multiplier(u32 value) : value(value){}
    xassume(for_plln, value);
private:
    u32 value;
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

xenumc(source_for_rtc)
    none,
    from_hse_div2 = 2, from_hse_div3, from_hse_div4, from_hse_div5, from_hse_div6, from_hse_div7, from_hse_div8, from_hse_div9, from_hse_div10, from_hse_div11, from_hse_div12, from_hse_div13, from_hse_div14, from_hse_div15, from_hse_div16, from_hse_div17, from_hse_div18, from_hse_div19, from_hse_div20, from_hse_div21, from_hse_div22, from_hse_div23, from_hse_div24, from_hse_div25, from_hse_div26, from_hse_div27, from_hse_div28, from_hse_div29, from_hse_div30, from_hse_div31,
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

xenumc(division_for_mco)
    none,
    div2 = 4,
    div3,
    div4,
    div5,
$;

xstruct(source_from_hsi)
    constexpr source_from_hsi(){}
    xassumex(source_for_pll , 0);
    xassumex(source_for_sclk, 0);
    xassumex(source_for_mco1, 0);
$;

xstruct(source_from_hse)
    constexpr source_from_hse(u32 value = 0) : value(value){}
    xassumex(source_for_pll , 1);
    xassumex(source_for_sclk, 1);
    xassumex(source_for_mco1, 2);
    xassumex(source_for_mco2, 2);
    xassumex(source_for_rtc , value);
private:
    u32 value;
$;

xstruct(source_from_pll)
    constexpr source_from_pll(){}
    xassumex(source_for_sclk, 2);
    xassumex(source_for_mco1, 3);
    xassumex(source_for_mco2, 3);
$;

xstruct(source_from_lse)
    constexpr source_from_lse(){}
    xassumex(source_for_mco1, 1);
$;

xstruct(source_from_plli2s)
    constexpr source_from_plli2s(){}
    xassumex(source_for_mco2, 1);
$;

xstruct(source_from_sclk)
    constexpr source_from_sclk(u32 value) : value(value){}
    xassumex(source_for_ahb, value);
    xassumex(source_for_mco2, 0);
private:
    u32 value;
$;

xstruct(source_from_ahb)
    constexpr source_from_ahb(u32 value) : value(value) {}
    xassumex(source_for_apb, value);
private:
    u32 value;
$;

constexpr source_from_sclk   from_sclk        { 0  };
constexpr source_from_sclk   from_sclk_div2   { 8  };
constexpr source_from_sclk   from_sclk_div4   { 9  };
constexpr source_from_sclk   from_sclk_div8   { 10 };
constexpr source_from_sclk   from_sclk_div16  { 11 };
constexpr source_from_sclk   from_sclk_div64  { 12 };
constexpr source_from_sclk   from_sclk_div128 { 13 };
constexpr source_from_sclk   from_sclk_div256 { 14 };
constexpr source_from_sclk   from_sclk_div512 { 15 };
constexpr source_from_ahb    from_ahb         { 0  };
constexpr source_from_ahb    from_ahb_div2    { 4  };
constexpr source_from_ahb    from_ahb_div4    { 5  };
constexpr source_from_ahb    from_ahb_div8    { 6  };
constexpr source_from_ahb    from_ahb_div16   { 7  };
constexpr source_from_hsi    from_hsi         {    };
constexpr source_from_hse    from_hse         {    };
constexpr source_from_pll    from_pll         {    };
constexpr source_from_plli2s from_plli2s      {    };

constexpr source_from_hse    none             { 0  };
constexpr source_from_hse    from_hse_div2    { 2  };
constexpr source_from_hse    from_hse_div3    { 3  };
constexpr source_from_hse    from_hse_div4    { 4  };
constexpr source_from_hse    from_hse_div5    { 5  };
constexpr source_from_hse    from_hse_div6    { 6  };
constexpr source_from_hse    from_hse_div7    { 7  };
constexpr source_from_hse    from_hse_div8    { 8  };
constexpr source_from_hse    from_hse_div9    { 9  };
constexpr source_from_hse    from_hse_div10   { 10 };
constexpr source_from_hse    from_hse_div11   { 11 };
constexpr source_from_hse    from_hse_div12   { 12 };
constexpr source_from_hse    from_hse_div13   { 13 };
constexpr source_from_hse    from_hse_div14   { 14 };
constexpr source_from_hse    from_hse_div15   { 15 };
constexpr source_from_hse    from_hse_div16   { 16 };
constexpr source_from_hse    from_hse_div17   { 17 };
constexpr source_from_hse    from_hse_div18   { 18 };
constexpr source_from_hse    from_hse_div19   { 19 };
constexpr source_from_hse    from_hse_div20   { 20 };
constexpr source_from_hse    from_hse_div21   { 21 };
constexpr source_from_hse    from_hse_div22   { 22 };
constexpr source_from_hse    from_hse_div23   { 23 };
constexpr source_from_hse    from_hse_div24   { 24 };
constexpr source_from_hse    from_hse_div25   { 25 };
constexpr source_from_hse    from_hse_div26   { 26 };
constexpr source_from_hse    from_hse_div27   { 27 };
constexpr source_from_hse    from_hse_div28   { 28 };
constexpr source_from_hse    from_hse_div29   { 29 };
constexpr source_from_hse    from_hse_div30   { 30 };
constexpr source_from_hse    from_hse_div31   { 31 };

xunion(rcc)
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
    $ items;

public:
    xunion()
        
    $ lse;

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
    $ hse;

    xunion()
        xrw(bool, on, pllon);
        xro(bool, ready, pllrdy);
        xunion()
            xrw(bool, on, plli2son);
            xrw(bool, ready, plli2srdy);
            xrw(source_for_i2s, source, i2ssrc);
        $ i2s;

        xunion()
            xrw(bool, on, pllsaion);
            xrw(bool, ready, pllsairdy);
        $ sai;

        // f(vco) = f(pll input) * (mul / div)
        // f(pll normal) = f(vco) / for_sclk
        // f(usb otg fs/sdio/random) = f(vco) / for_otg_sdio_random
        xunion()
            // division for vco input frequency
            // notice:
            // div must in range [2, 63]
            xrw(division::for_pllm, div, pllm);

            // multiplier for vco output frequency
            // notice:
            // vco input freq * mul must in range [192, 432] MHz
            xrw(multiplier::for_plln, mul, plln);

            // division for main system clock(sclk) when pll as the sclk
            // notice:
            // make sure sclk freqence no more than 180MHz
            // only 2, 4, 6, 8 is legal
            xrw(division::for_pllp, for_sclk, pllp);

            // division for usb otg fs/sdio/random
            // notice:
            // usb otg fs need 48MHz frequenc
            // sdio/random need less equal than 48MHz
            // for_otg_sdio_random must in range [2, 15]
            xrw(division::for_otg_sdio_random, for_otg_sdio_random, pllq);
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
        // divsion from hse
        // rtc clock need equals to 1MHz
        xrw(source_for_rtc, source, rtcpre);
    $ rtc;

    xunion()
        xrw(source_for_mco1, source, mco1);
        xrw(division_for_mco, freq, mco1pre);
    $ mco1;

    xunion()
        xrw(source_for_mco2, source, mco2);
        xrw(division_for_mco, freq, mco2pre);
    $ mco2;

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
    xport(crc  );

    xport_lpen(axi  );
    xport_lpen(flash);
    xport_lpen(sram1);
    xport_lpen(sram2);

    xport_en_lpen(backup_sram);
    xport_en_lpen(tdcm_ram   );

    xport(dma1   );
    xport(dma2   );
    xport(dma2d  );
    xport(eth    );
    xport_en_lpen(eth_tx);
    xport_en_lpen(eth_rx);
    xport_en_lpen(eth_ptp);
    xport(otghs  );
    xport_en_lpen(otghs_ulpi);
    xport(dcmi   );
    xport(crypto );
    xport(hash   );
    xport(random );
    xport(otgfs  );
    xport(fmc    );
    xport(qspi   );
    xport(tim2   );
    xport(tim3   );
    xport(tim4   );
    xport(tim5   );
    xport(tim6   );
    xport(tim7   );
    xport(tim12  );
    xport(tim13  );
    xport(tim14  );
    xport(lptim1 );
    xport(wwdg   );
    xport(spi2   );
    xport(spi3   );
    xport(spdifrx);
    xport(uart2  );
    xport(uart3  );
    xport(uart4  );
    xport(uart5  );
    xport(i2c1   );
    xport(i2c2   );
    xport(i2c3   );
    xport(i2c4   );
    xport(can1   );
    xport(can2   );
    xport(cec    );
    xport(pwr    );
    xport(dac    );
    xport(uart7  );
    xport(uart8  );
    xport(tim1   );
    xport(tim8   );
    xport(usart1 );
    xport(usart6 );
    
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

