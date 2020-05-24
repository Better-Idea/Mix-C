#ifndef xpack_extern_stm32_setup
#define xpack_extern_stm32_setup
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::extern_stm32_setup
        #include"extern/stm32/define.hpp"
        #include"extern/stm32/rcc.hpp"
        #include"extern/stm32/fls.hpp"
        #include"extern/stm32/pwr.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::extern_stm32_setup::origin{
        inline void stm32_setup(){
            using namespace inc;
            using namespace mixc;

            rcc.pwr.enable(true);
            rcc.flush();

            pwr.regulator_scaling.at(pwr_regulator_scaling::lv1);
            pwr.flush();

            rcc.pll
                .source(rcc_source_for_pll::from_hse)
                .freq.div(8).mul(432)
                .for_sclk(rcc_for_sclk::div2)
                .for_otg_sdio_random(rcc_for_otg_sdio_random::div2);
            rcc.hse.enable(true);
            rcc.flush();
            while(not rcc.hse.ready());

            rcc.pll.enable(true);
            rcc.flush();
            while(not rcc.pll.ready());

            pwr.over_drive.enable(true);
            pwr.flush();
            while(not pwr.over_drive.ready());

            pwr.over_drive.enable_switch(true);
            pwr.flush();
            while(not pwr.over_drive.ready_switch());

            fls.latency(7).flush();

            rcc.sclk.source(rcc_source_for_sclk::from_pll);
            rcc.ahb.source(rcc_source_for_ahb::from_sclk);
            rcc.apb1.source(rcc_source_for_apb::from_ahb_div4); // no more than 54MHz  (216 / 4)
            rcc.apb2.source(rcc_source_for_apb::from_ahb_div2); // no more than 108MHz (216 / 2)
            rcc.flush();
        }
    }

#endif
