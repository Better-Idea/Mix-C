#ifndef xpack_extern_stm32_pwr
#define xpack_extern_stm32_pwr
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::extern_stm32_pwr
        #include"extern/stm32/define.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::extern_stm32_pwr::origin{
        xmmio(_pwr){
        private:
            enum{
                xfield(lpds        , 1),
                xfield(pdds        , 1),
                xrsv(1),
                xfield(csbf        , 1),
                xfield(pvde        , 1),
                xfield(pls         , 3),
                xfield(dbp         , 1),
                xfield(fpds        , 1),
                xfield(lpuds       , 1),
                xfield(mruds       , 1),
                xrsv(1),
                xfield(adcdc1      , 1),
                xfield(vos         , 2),
                xfield(oden        , 1),
                xfield(odswen      , 1),
                xfield(uden        , 2),
                xrsv(12),

                xfield(wuif        , 1),
                xfield(sbf         , 1),
                xfield(pvdo        , 1),
                xfield(brr         , 1),
                xfield(eiwup       , 1),
                xrsv(4),
                xfield(bre         , 1),
                xrsv(4),
                xfield(vosrdy      , 1),
                xrsv(1),
                xfield(odrdy       , 1),
                xfield(odswrdy     , 1),
                xfield(udrdy       , 2),
                xrsv(12),

                xfield(cwupf1      , 1),
                xfield(cwupf2      , 1),
                xfield(cwupf3      , 1),
                xfield(cwupf4      , 1),
                xfield(cwupf5      , 1),
                xfield(cwupf6      , 1),
                xrsv(2),
                xfield(wupp1       , 1),
                xfield(wupp2       , 1),
                xfield(wupp3       , 1),
                xfield(wupp4       , 1),
                xfield(wupp5       , 1),
                xfield(wupp6       , 1),
                xrsv(2 + 16),

                xfield(wupf1       , 1),
                xfield(wupf2       , 1),
                xfield(wupf3       , 1),
                xfield(wupf4       , 1),
                xfield(wupf5       , 1),
                xfield(wupf6       , 1),
                xrsv(2),
                xfield(ewup1       , 1),
                xfield(ewup2       , 1),
                xfield(ewup3       , 1),
                xfield(ewup4       , 1),
                xfield(ewup5       , 1),
                xfield(ewup6       , 1),
                xrsv(2 + 16),

                field_bits,
            };
        public:
            xinner(_pwr);

            union{
                union{
                    xrw(pwr_source_regulator, source, lpds)
                    xrw(pwr_regulator, low_power, lpuds)
                    xrw(pwr_regulator, main, mruds)
                } regulator;

                xrw(pwr_flash, flash, fpds);

                union{
                    xrwx(bool, enable,
                        (xget(uden) == 0x3), 
                        (xset(uden, value ? 0x3 : 0))
                    )

                    xrox(bool, ready, (xget(udrdy) == 0x3))
                    xwox(clear_t, ready, (xset(udrdy, 0x3)))  // write to clear
                } under_drive;
            } stop_mode;

            xrw(pwr_deep_sleep, deep_sleep, pdds)

            // power voltage detector
            union{
                xrw(bool, enable, pvde)
                xro(bool, higher_than_vdd, pvdo)
                xrw(pwr_thresholds_voltage, thresholds_voltage, pls)
            } pvd;

            xrw(pwr_backup_domain, backup_domain, dbp)

            // TODO:ADCDC1 ==========================================================================
            // xrw(xx, adcdc1, adcdc1);

            union{
                // can be modifed only when PLL is OFF
                // active at PLL ON
                xrw(pwr_regulator_scaling, at, vos)
                xro(bool, ready, vosrdy)
            } regulator_scaling;

            union{
                xro(bool, ready, brr);

                // used to maintain backup SRAM content in standby and vbat modes
                // the content which in backup SRAM will lose when disable the backup_regulator.
                xrw(bool, enable, bre);
            } backup_regulator;

            union{
                // HSI or HSE must be selected as system clock before set this bit
                xrw(bool, enable, oden)

                // set this bit to switching to over drive mode
                // need wait over_drive.ready bit equals to 1 before
                xrw(bool, enable_switch, odswen)

                xro(bool, ready, odrdy)
                xro(bool, ready_switch, odswrdy)
            } over_drive;

            xrwx(bool, standby, 
                (xget(sbf)), 
                (xset(sbf, 1))
            )

            // wakeup from internal RTC 
            union{
                xro(bool, occur, wuif);
                xrw(bool, enable, eiwup);
            } wakeup_internal;

            union{
                #define xgen(pin,id)                                    \
                union{                                                  \
                    xrw(pwr_wu_polarity, polarity, xlink2(wupp, id))    \
                    xrw(bool, enable, xlink2(ewup, id))                 \
                    xrwx(bool, occur,                                   \
                        (xget(xlink2(wupf, id))),                       \
                        (xset(xlink2(cwupf, id), 1))                    \
                    )                                                   \
                } pin

                xgen(pa0,  1);
                xgen(pa2,  2);
                xgen(pc1,  3);
                xgen(pc13, 4);
                xgen(pi8,  5);
                xgen(pi11, 6);
                #undef  xgen
            } wakeup_extern;
        };

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunused-variable"
            inline static _pwr<0x40007000> pwr;
        #pragma GCC diagnostic pop
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::extern_stm32_pwr::origin;
}

