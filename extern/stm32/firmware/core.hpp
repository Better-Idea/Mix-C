#pragma once
#include"definition/mini.hpp"

#define xro(type,name,target)                 \
    union _ ## name{                          \
        operator type(){                      \
            field * ptr = (field *)this;      \
            return type((u32)ptr->target);    \
        }                                     \
    }name

#define xrw(type,name,target)                 \
    union _ ## name{                          \
        operator type(){                      \
            field * ptr = (field *)this;      \
            return type((u32)ptr->target);    \
        }                                     \
        void operator=(type value){           \
            field * ptr = (field *)this;      \
            ptr->target = u32(ptr->target);   \
        }                                     \
    } name;


constexpr bool enable = true;
constexpr bool disable = false;

#define xassume(type,value)                   \
    xenumc(type) $;                           \
    xcvtc(type)                               \
        return type(value);                   \
    $

#define xassumex(type,value)                  \
    xcvtc(type)                               \
        return type(value);                   \
    $

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
    from_hse_div2 = 2,
    from_hse_div3,
    from_hse_div4,
    from_hse_div5,
    from_hse_div6,
    from_hse_div7,
    from_hse_div8,
    from_hse_div9,
    from_hse_div10,
    from_hse_div11,
    from_hse_div12,
    from_hse_div13,
    from_hse_div14,
    from_hse_div15,
    from_hse_div16,
    from_hse_div17,
    from_hse_div18,
    from_hse_div19,
    from_hse_div20,
    from_hse_div21,
    from_hse_div22,
    from_hse_div23,
    from_hse_div24,
    from_hse_div25,
    from_hse_div26,
    from_hse_div27,
    from_hse_div28,
    from_hse_div29,
    from_hse_div30,
    from_hse_div31,
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

xstruct(source_from_hsi)
    constexpr source_from_hsi(){}
    xassumex(source_for_pll, 0);
    xassumex(source_for_sclk, 0);
$;

xstruct(source_from_hse)
    constexpr source_from_hse(u32 value = 0) : value(value){}
    xassumex(source_for_pll, 1);
    xassumex(source_for_sclk, 1);
    xassumex(source_for_rtc, value);
private:
    u32 value;
$;

xstruct(source_from_pll)
    constexpr source_from_pll(){}
    xassumex(source_for_sclk, 2);
$;

xstruct(source_from_sclk)
    constexpr source_from_sclk(u32 value) : value(value){}
    xassumex(source_for_ahb, value);
private:
    u32 value;
$;

xstruct(source_from_ahb)
    constexpr source_from_ahb(u32 value) : value(value) {}
    xassumex(source_for_apb, value);
private:
    u32 value;
$;

constexpr source_from_sclk from_sclk        {  0 };
constexpr source_from_sclk from_sclk_div2   {  8 };
constexpr source_from_sclk from_sclk_div4   {  9 };
constexpr source_from_sclk from_sclk_div8   { 10 };
constexpr source_from_sclk from_sclk_div16  { 11 };
constexpr source_from_sclk from_sclk_div64  { 12 };
constexpr source_from_sclk from_sclk_div128 { 13 };
constexpr source_from_sclk from_sclk_div256 { 14 };
constexpr source_from_sclk from_sclk_div512 { 15 };
constexpr source_from_ahb  from_ahb         {  0 };
constexpr source_from_ahb  from_ahb_div2    {  4 };
constexpr source_from_ahb  from_ahb_div4    {  5 };
constexpr source_from_ahb  from_ahb_div8    {  6 };
constexpr source_from_ahb  from_ahb_div16   {  7 };
constexpr source_from_hsi  from_hsi         {    };
constexpr source_from_hse  from_hse         {    };
constexpr source_from_pll  from_pll         {    };

constexpr source_from_hse  none             { 0  };
constexpr source_from_hse  from_hse_div2    { 2  };
constexpr source_from_hse  from_hse_div3    { 3  };
constexpr source_from_hse  from_hse_div4    { 4  };
constexpr source_from_hse  from_hse_div5    { 5  };
constexpr source_from_hse  from_hse_div6    { 6  };
constexpr source_from_hse  from_hse_div7    { 7  };
constexpr source_from_hse  from_hse_div8    { 8  };
constexpr source_from_hse  from_hse_div9    { 9  };
constexpr source_from_hse  from_hse_div10   { 10 };
constexpr source_from_hse  from_hse_div11   { 11 };
constexpr source_from_hse  from_hse_div12   { 12 };
constexpr source_from_hse  from_hse_div13   { 13 };
constexpr source_from_hse  from_hse_div14   { 14 };
constexpr source_from_hse  from_hse_div15   { 15 };
constexpr source_from_hse  from_hse_div16   { 16 };
constexpr source_from_hse  from_hse_div17   { 17 };
constexpr source_from_hse  from_hse_div18   { 18 };
constexpr source_from_hse  from_hse_div19   { 19 };
constexpr source_from_hse  from_hse_div20   { 20 };
constexpr source_from_hse  from_hse_div21   { 21 };
constexpr source_from_hse  from_hse_div22   { 22 };
constexpr source_from_hse  from_hse_div23   { 23 };
constexpr source_from_hse  from_hse_div24   { 24 };
constexpr source_from_hse  from_hse_div25   { 25 };
constexpr source_from_hse  from_hse_div26   { 26 };
constexpr source_from_hse  from_hse_div27   { 27 };
constexpr source_from_hse  from_hse_div28   { 28 };
constexpr source_from_hse  from_hse_div29   { 29 };
constexpr source_from_hse  from_hse_div30   { 30 };
constexpr source_from_hse  from_hse_div31   { 31 };

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

    $;

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
    $ mco1;
$;

xopr(division, "" div, u64 value)
    return division(u32(value));
$

xopr(multiplier, "" mul, u64 value)
    return multiplier(u32(value));
$

