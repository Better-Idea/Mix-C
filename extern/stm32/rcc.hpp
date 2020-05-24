#ifndef xpack_extern_stm32_rcc
#define xpack_extern_stm32_rcc
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::extern_stm32_rcc
        #include"extern/stm32/define.hpp"
        #include"meta/is_same.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::extern_stm32_rcc::origin{
        // read/clear/enable/disable
        #define xevent(name,flag,clear,enable)                                          \
            union {                                                                     \
                operator bool(){                                                        \
                    return bool(xget(flag));                                            \
                }                                                                       \
                void operator=(bool value){                                             \
                    xset(enable, value);                                                \
                }                                                                       \
                void operator=(clear_t){                                                \
                    xset(clear, 1);                                                     \
                }                                                                       \
            } name;

        #define xport(name)                                                             \
        union{                                                                          \
            xrw(bool, reset, xlink(name, rst))                                          \
            xrw(bool, enable, xlink(name, en))                                          \
            xrw(bool, always_active, xlink(name, lpen))                                 \
        } name;

        #define xport_en_lp(name)                                                       \
        union{                                                                          \
            xrw(bool, enable, xlink(name, en))                                          \
            xrw(bool, always_active, xlink(name, lpen))                                 \
        } name;

        #define xport_rst(name)                                                         \
        union{                                                                          \
            xrw(bool, reset, xlink(name, rst))                                          \
        } name;

        #define xport_lp(name)                                                          \
        union{                                                                          \
            xrw(bool, always_active, xlink(name, lpen))                                 \
        } name;

        xmmio(_rcc) {
        private:
            enum{
                xfield(hsion        , 1),
                xfield(hsirdy       , 1),
                xrsv(1),
                xfield(hsitrim      , 5),
                xfield(hsical       , 8),
                xfield(hseon        , 1),
                xfield(hserdy       , 1),
                xfield(hsebyp       , 1),
                xfield(csson        , 1),
                xrsv(4),
                xfield(pllon        , 1),
                xfield(pllrdy       , 1),
                xfield(plli2son     , 1),
                xfield(plli2srdy    , 1),
                xfield(pllsaion     , 1),
                xfield(pllsairdy    , 1),
                xrsv(2),

                xfield(pllm         , 6),
                xfield(plln         , 9),
                xrsv(1),
                xfield(pllp         , 2),
                xrsv(4),
                xfield(pllsrc       , 1),
                xrsv(1),
                xfield(pllq         , 4),
                xrsv(4),

                xfield(sw           , 2),
                xfield(sws          , 2),
                xfield(hpre         , 4),
                xrsv(2),
                xfield(ppre1        , 3),
                xfield(ppre2        , 3),
                xfield(rtcpre       , 5),
                xfield(mcoa         , 2),
                xfield(i2ssrc       , 1),
                xfield(mcoapre      , 3),
                xfield(mcobpre      , 3),
                xfield(mcob         , 2),

                xfield(lsirdyf      , 1),
                xfield(lserdyf      , 1),
                xfield(hsirdyf      , 1),
                xfield(hserdyf      , 1),
                xfield(pllrdyf      , 1),
                xfield(plli2srdyf   , 1),
                xfield(pllsairdyf   , 1),
                xfield(cssf         , 1),
                xfield(lsirdyie     , 1),
                xfield(lserdyie     , 1),
                xfield(hsirdyie     , 1),
                xfield(hserdyie     , 1),
                xfield(pllrdyie     , 1),
                xfield(plli2srdyie  , 1),
                xfield(pllsairdyie  , 1),
                xrsv(1),
                xfield(lsirdyc      , 1),
                xfield(lserdyc      , 1),
                xfield(hsirdyc      , 1),
                xfield(hserdyc      , 1),
                xfield(pllrdyc      , 1),
                xfield(plli2srdyc   , 1),
                xfield(pllsairdyc   , 1),
                xfield(cssc         , 1),
                xrsv(8),

                #define xarg 0
                #include"extern/stm32/private/gen_rcc.hpp"

                #define xarg 1
                #include"extern/stm32/private/gen_rcc.hpp"

                #define xarg 2
                #include"extern/stm32/private/gen_rcc.hpp"

                xfield(lseon        , 1),
                xfield(lserdy       , 1),
                xfield(lsebyp       , 1),
                xfield(lsedrv       , 2),
                xrsv(3),
                xfield(rtcsel       , 2),
                xrsv(5),
                xfield(rtcen        , 1),
                xfield(bdrst        , 1),
                xrsv(15),

                xfield(lsion        , 1),
                xfield(lsirdy       , 1),
                xrsv(14 + 8),
                xfield(rmvf         , 1),
                xfield(borrstf      , 1),
                xfield(pinrstf      , 1),
                xfield(porrstf      , 1),
                xfield(sftrstf      , 1),
                xfield(iwdgrstf     , 1),
                xfield(wwdgrstf     , 1),
                xfield(lpwrrstf     , 1),

                xrsv(32),
                xrsv(32),

                xfield(modper       , 13),
                xfield(incstep      , 15),
                xrsv(2),
                xfield(spreadsel    , 1),
                xfield(sscgen       , 1),
                // to be continue...

                field_bits,
            };
        public:
            xinner(_rcc)

            xrw(bool, reset_backup_domain, bdrst)

            // TODO:comment
            union{
                xrw(u32, period, modper)
                xrw(u32, step, incstep)
                xrw(rcc_spread_mode, mode, spreadsel)
                xrw(bool, enable, sscgen)
            } spread_spectrum;

            union{
                xro(bool, bor_reset, borrstf)
                xro(bool, pin_reset, pinrstf)
                xro(bool, por_reset, porrstf)
                xro(bool, software_reset, sftrstf)
                xro(bool, iwdg_reset, iwdgrstf)
                xro(bool, wwdg_reset, wwdgrstf)
                xro(bool, lpwr_reset, lpwrrstf)

                void clear_all(){
                    xset(rmvf, true);
                }
            } flag;

            union{
                xrw(bool, enable, lseon)
                xro(bool, ready, lserdy)
                xrw(bool, bypass, lsebyp)
                xrw(rcc_drive_capability, drive_capability, lsedrv)
            } lse;

            union{
                xrw(bool, enable, lsion)
                xro(bool, ready, lsirdy)
            } lsi;

            union{
                xrw(bool, enable, hsion)
                xro(bool, ready, hsirdy)
                xrw(u32 , triming, hsitrim)
                xro(u32 , calibration, hsical)
            } hsi;

            union{
                xrw(bool, enable, hseon)
                xro(bool, ready, hserdy)
                xrw(bool, bypass, hsebyp)
                xrw(bool, use_safe_clock, csson)

                // divsion from hse
                // rtc clock need equals to 1MHz
                xrw(rcc_for_rtc, for_rtc, rtcpre)
            } hse;

            union{
                xrw(bool, enable, plli2son)
                xro(bool, ready, plli2srdy)
                xrw(rcc_source_for_i2s, source, i2ssrc)
            } i2s;

            union{
                xrw(bool, enable, pllsaion)
                xro(bool, ready, pllsairdy)
            } sai;

            union{
                xrw(bool, enable, pllon)
                xro(bool, ready, pllrdy)

                // f(vco) = f(pll input) * (mul / div)
                // f(pll normal) = f(vco) / for_sclk
                // f(usb otg fs/sdio/random) = f(vco) / for_otg_sdio_random
                union{
                    // division for vco input frequency
                    // notice:
                    // div must in range [2, 63]
                    xrw(u32, div, pllm)

                    // multiplier for vco output frequency
                    // notice:
                    // vco input freq * mul must in range [192, 432] MHz
                    xrw(u32, mul, plln)

                    // division for main system clock(sclk) when pll as the sclk
                    // notice:
                    // make sure sclk freqence no more than 180MHz
                    // only 2, 4, 6, 8 is legal
                    xrw(rcc_for_sclk, for_sclk, pllp)

                    // division for usb otg fs/sdio/random
                    // notice:
                    // usb otg fs need 48MHz frequence
                    // sdio/random need less equal than 48MHz
                    // for_otg_sdio_random must in range [2, 15]
                    xrw(rcc_for_otg_sdio_random, for_otg_sdio_random, pllq)
                } freq;

                xrw(rcc_source_for_pll, source, pllsrc)
            } pll;

            union{
                xrw(rcc_source_for_sclk, source, sw)
                xro(rcc_source_for_sclk, current_source, sws)
            } sclk;

            union{
                // division from sclk
                // ahb clock not less than 25MHz
                xrw(rcc_source_for_ahb, source, hpre)
            } ahb;

            union{
                // division from ahb
                // apb clock no more than 54MHz
                xrw(rcc_source_for_apb, source, ppre1)
            } apb1;

            union{
                // division from ahb
                // apb clock no more than 108MHz
                xrw(rcc_source_for_apb, source, ppre2)
            } apb2;

            union{
                xrw(bool, enable, rtcen)
                xrw(rcc_source_for_rtc, source, rtcsel)
            } rtc;

            union{
                xrw(rcc_source_for_mco1, source, mcoa)
                xrw(rcc_for_mco, freq, mcoapre)
            } mco1;

            union{
                xrw(rcc_source_for_mco2, source, mcob)
                xrw(rcc_for_mco, freq, mcobpre)
            } mco2;

            // those property can be set clear/enable/disable
            // and read for check whether the event is occur.
            union{
                xevent(lsi_ready   , lsirdyf   , lsirdyc   , lsirdyie)
                xevent(lse_ready   , lserdyf   , lserdyc   , lserdyie)
                xevent(hsi_ready   , hsirdyf   , hsirdyc   , hsirdyie)
                xevent(hse_ready   , hserdyf   , hserdyc   , hserdyie)
                xevent(pll_ready   , pllrdyf   , pllrdyc   , pllrdyie)
                xevent(plli2s_ready, plli2srdyf, plli2srdyc, plli2srdyie)
                xevent(pllsai_ready, pllsairdyf, pllsairdyc, pllsairdyie)
                xevent(hse_broken  , cssf      , cssc      , csson)

                auto & clear_all(){
                    lsi_ready    = clear;
                    lse_ready    = clear;
                    hsi_ready    = clear;
                    hse_ready    = clear;
                    pll_ready    = clear;
                    plli2s_ready = clear;
                    pllsai_ready = clear;
                    hse_broken   = clear;
                    return this[0];
                }
            } event;

            xport(gpioa)
            xport(gpiob)
            xport(gpioc)
            xport(gpiod)
            xport(gpioe)
            xport(gpiof)
            xport(gpiog)
            xport(gpioh)
            xport(gpioi)
            xport(gpioj)
            xport(gpiok)
            xport(crc)

            xport_lp(axi)
            xport_lp(flash)
            xport_lp(sram1)
            xport_lp(sram2)

            xport_en_lp(backup_sram)
            xport_en_lp(tdcm_ram)

            xport(dma1)
            xport(dma2)
            xport(dma2d)
            xport(eth)
            xport_en_lp(eth_tx)
            xport_en_lp(eth_rx)
            xport_en_lp(eth_ptp)
            xport(otghs)
            xport_en_lp(otghs_ulpi)

            xport(dcmi)
            xport(crypto)
            xport(hash)
            xport(random)
            xport(otgfs)
            xport(fmc)
            xport(qspi)
            xport(tim2)
            xport(tim3)
            xport(tim4)
            xport(tim5)
            xport(tim6)
            xport(tim7)
            xport(tim12)
            xport(tim13)
            xport(tim14)
            xport(lptim1)
            xport(wwdg)
            xport(spi2)
            xport(spi3)
            xport(spdif_rx)
            xport(uart2)
            xport(uart3)
            xport(uart4)
            xport(uart5)
            xport(i2c1)
            xport(i2c2)
            xport(i2c3)
            xport(i2c4)
            xport(can1)
            xport(can2)
            xport(cec)
            xport(pwr)
            xport(dac)
            xport(uart7)
            xport(uart8)
            xport(tim1)
            xport(tim8)
            xport(usart1)
            xport(usart6)

            xport_rst(adc)
            xport_en_lp(adc1)
            xport_en_lp(adc2)
            xport_en_lp(adc3)

            xport(sdmmc1)
            xport(spi1)
            xport(spi4)
            xport(syscfg)
            xport(tim9)
            xport(tim10)
            xport(tim11)
            xport(spi5)
            xport(spi6)
            xport(sai1)
            xport(sai2)
            xport(ltdc)
        };

        #undef  xevent
        #undef  xport
        #undef  xport_lp
        #undef  xport_rst
        #undef  xport_en_lp


        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunused-variable"
            inline static _rcc<0x40023800> rcc;
        #pragma GCC diagnostic pop
    }

#endif

namespace xuser::inc{
    using namespace mixc::extern_stm32_rcc::origin;
}
