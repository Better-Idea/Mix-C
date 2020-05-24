#ifndef xpack_extern_stm32_fls
#define xpack_extern_stm32_fls
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::extern_stm32_fls
        #include"define/bitbind.hpp"
        #include"extern/stm32/define.hpp"
        #include"instruction/index_of_first_set.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::extern_stm32_fls::origin{
        enum class fls_read_protect{
            none    = 0xaa,
            ic      = 0xcc,
            memory  = 0xff,
        };

        enum class fls_wgd_type{
            hardware,
            software,
        };

        enum class fls_bor_level{
            lv3,
            lv2,
            lv1,
            none,
        };

        enum class fls_boot_from{          
            // 0x00000000
            from_itcm_ram    = 0x0000,

            // 0x00100000
            from_sys_ram     = 0x0040,

            // 0x00200000
            from_itcm_flash  = 0x0080,

            // 0x08000000
            from_axim_flash  = 0x2000,

            // 0x20000000
            from_dtcm_ram    = 0x8000,

            // 0x20010000
            from_sram1       = 0x8004,

            // 0x2004C000
            from_sram2       = 0x8013,
        };

        xmmio(_fls){
        private:
            // // option
            // enum{
            //     // address:0x1FFF0000
            //     // default:0x5500AAFF
            //     xrsv(2),
            //     xfield(bor_lev      , 2),
            //     xfield(wwdg_sw      , 1),
            //     xfield(iwdg_sw      , 1),
            //     xfield(nrst_stop    , 1),
            //     xfield(nrst_stdby   , 1),
            //     xfield(rdp          , 8),
            //     xrsv(16),

            //     xrsv(32),

            //     // address:0x1FFF0008
            //     // default:0x0000FFFF
            //     xfield(nwrpi        , 8),
            //     xrsv(6),
            //     xfield(iwdg_stdby   , 1),
            //     xfield(iwdg_stop    , 1),
            //     xrsv(16),

            //     xrsv(32),

            //     // address:0x1FFF0010
            //     // default:0xFF7F0080 ITCM_FLASH base address
            //     xfield(boot_add0    , 16),
            //     xrsv(16),

            //     xrsv(32),

            //     // address:0x1FFF0018
            //     // default:0xFFBF0040 system base address
            //     xfield(boot_add1    , 16),
            //     xrsv(16),
            // };

            enum{
                xfield(latency_acr  , 4),
                xrsv(4),
                xfield(prften       , 1),
                xfield(arten        , 1),
                xrsv(1),
                xfield(artrst       , 1),
                xrsv(20),

                xfield(keyr         , 32),

                xfield(optkeyr      , 32),

                xfield(eop          , 1),
                xfield(operr        , 1),
                xrsv(2),
                xfield(wrperr       , 1),
                xfield(pgaerr       , 1),
                xfield(pgperr       , 1),
                xfield(erserr       , 1),
                xrsv(8),
                xfield(bsy          , 1),
                xrsv(15),

                xfield(pg           , 1),
                xfield(ser          , 1),
                xfield(mer          , 1),
                xfield(snb          , 4),
                xrsv(1),
                xfield(psize        , 2),
                xrsv(6),
                xfield(strt         , 1),
                xrsv(7),
                xfield(eopie        , 1),
                xfield(errie        , 1),
                xrsv(5),
                xfield(lockcr       , 1),

                xfield(optlock      , 1),
                xfield(optstrt      , 1),
                xfield(bor_lev      , 2),
                xfield(wwdg_sw      , 1),
                xfield(iwdg_sw      , 1),
                xfield(nrst_stop    , 1),
                xfield(nrst_stdby   , 1),
                xfield(rdp          , 8),
                xfield(nwrp         , 8),
                xrsv(6),
                xfield(iwdg_stdby   , 1),
                xfield(iwdg_stop    , 1),

                xfield(boot_add0    , 16),
                xfield(boot_add1    , 16),

                field_bits,
            };

        public:
            xinner(_fls)

            xrw(bool, enable_art, arten)
            xrw(bool, enable_prefetch, prften)
            xrw(bool, reset_art, artrst)
            xrw(u32, latency, latency_acr)

            auto & unlock(){
                xset(keyr, 0x45670123); the_t::flush();
                xset(keyr, 0xcdef89ab); the_t::flush();
                return the;
            }

            auto & lock(){
                xset(lockcr, 1); the_t::flush();
                return the;
            }

            union _opt{
                using the_t = _opt;

                auto & unlock(){
                    xset(optkeyr, 0x08192a3b); the_t::flush();
                    xset(optkeyr, 0x4c5d6e7f); the_t::flush();
                    return the;
                }

                auto & lock(){
                    xset(optlock, 1);
                    return the;
                }

                union{
                    xrw(bool, freeze_at_stop_mode, iwdg_stop)
                    xrw(bool, freeze_at_standby, iwdg_stdby)
                    xrw(fls_wgd_type, type, iwdg_sw)
                } iwdg;

                union{
                    xrw(fls_wgd_type, type, wwdg_sw)
                } wwdg;

                union _sector_write_protect{
                    auto & all(){
                        xset(nwrp, 0x00);
                        return the;
                    }

                    auto & disable(){
                        xset(nwrp, 0xff);
                        return the;
                    }

                    template<class ... args>
                    auto & any(u32 index, args ... list){
                        xset(nwrp, ~mask(index, list...));
                        return the;
                    }

                    template<class ... args>
                    auto & except(u32 index, args ... list){
                        xset(nwrp, mask(index, list...));
                        return the;
                    }
                private:
                    template<class ... args>
                    auto mask(args ... list){
                        u32 gorup[] = { list... };
                        u32 r = 0;

                        for(u32 i = 0; i < sizeof...(args); i++){
                            r |= 1 << gorup[i];
                        }
                        return r;
                    }
                } sector_write_protect;

                xrw(fls_read_protect, read_protect, rdp)

                xrw(bool, reset_at_stop_mode, nrst_stop)
                xrw(bool, reset_at_standby, nrst_stdby)

                xrw(fls_bor_level, bor, bor_lev)

                auto & effect(){
                    xset(optstrt, 1);
                    return the;
                }

                xrw(fls_boot_from, boot0, boot_add0)
                xrw(fls_boot_from, boot1, boot_add1)
            } opt;

            xro(bool, busy, bsy)

            union{
                xro(bool, erase_sequence, erserr)
                xro(bool, programming_bit_width, erserr)
                xr1(bool, programming_alignment, pgaerr)
                xr1(bool, write_protection, wrperr)
                xr1(bool, occur, operr)
            } error;
            
            xro(bool, finished, eop)

            xrw(bool, enable_error_interrupt, errie)
            xrw(bool, enable_finished_interrupt, eopie)

            union{
                xrw(bool, start, strt)
                xrwx(u32, bit_width, 
                    (1 << (xget(psize) + 3)),
                    (xset(psize, inc::index_of_first_set(value)))
                )
                xrw(u32, sector, snb)
                xrw(bool, mass, mer)
                xrw(bool, enable, ser)
                xrw(bool, programming, pg)
            } erase;
        };


        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunused-variable"
            inline static _fls<0x40023c00> fls;
        #pragma GCC diagnostic pop
    }
#endif

namespace xuser::inc{
    using namespace ::mixc::extern_stm32_fls::origin;
}
