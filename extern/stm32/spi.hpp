#ifndef xpack_extern_stm32_spi
#define xpack_extern_stm32_spi
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::extern_stm32_spi
        #include"extern/stm32/define.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::extern_stm32_spi::origin{
        xmmio(_spi){
        private:
            enum{
                xfield(cpha        , 1),
                xfield(cpol        , 1),
                xfield(mstr        , 1),
                xfield(br          , 3),
                xfield(spe         , 1),
                xfield(lsbfirst    , 1),
                xfield(ssi         , 1),
                xfield(ssm         , 1),
                xfield(rxonly      , 1),
                xfield(crcl        , 1),
                xfield(crcnext     , 1),
                xfield(crcen       , 1),
                xfield(bidioe      , 1),
                xfield(bidimode    , 1),
                xrsv(16),

                xfield(rxdmaen     , 1),
                xfield(txdmaen     , 1),
                xfield(ssoe        , 1),
                xfield(nssp        , 1),
                xfield(frf         , 1),
                xfield(errie       , 1),
                xfield(rxneie      , 1),
                xfield(txeie       , 1),
                xfield(ds          , 4),
                xfield(frxth       , 1),
                xfield(ldma_rx     , 1),
                xfield(ldma_tx     , 1),
                xrsv(17),

                xfield(rxne        , 1),
                xfield(txe         , 1),
                xfield(chside      , 1),
                xfield(udr         , 1),
                xfield(crcerr      , 1),
                xfield(modf        , 1),
                xfield(ovr         , 1),
                xfield(bsy         , 1),
                xfield(fre         , 1),
                xfield(frlvl       , 2),
                xfield(ftlvl       , 2),
                xrsv(19),

                xfield(dr16        , 16),
                xrsv(16),

                xfield(crcpr       , 16),
                xrsv(16),

                xfield(rxcrcr      , 16),
                xrsv(16),

                xfield(txcrcr      , 16),
                xrsv(16),

                field_bits,
            };

            enum{
                dr8     = dr16,
                end_dr8 = dr8 + 8 - 1,
            };

        public:
            xinner(_spi);

            auto & mode(spi_full_duplex){
                xset(bidimode, 0);
                xset(rxonly  , 0);
                return the;
            }

            auto & mode(spi_full_duplex, spi_transmit_only){
                return mode(full_duplex);
            }

            auto & mode(spi_full_duplex, spi_receive_only){
                xset(bidimode, 0);
                xset(rxonly  , 1);
                return the;
            }

            auto & mode(spi_half_duplex, spi_transmit_only){
                xset(bidimode , 1);
                xset(bidioe   , 1);
                return the;
            }

            auto & mode(spi_half_duplex, spi_receive_only){
                xset(bidimode , 1);
                xset(bidioe   , 0);
                return the;
            }

            xrw(bool, internal_slave_select, ssi)
            xrw(bool, software_slave_management, ssm)

            union{
                xrw(bool, enable, crcen)
                xrw(bool, load, crcnext)
                xrw(spi_crc_length, type, crcl)
            } crc;

            xrw(spi_bit_order, bit_order, lsbfirst)
            xrw(bool, enable, spe)
            xrw(spi_baud_rate, baud_rate, br)
            xrw(spi_role, role, mstr)
            xrw(u32, clock_polarity, cpol)
            xrw(u32, phase, cpha)

            // TODO, ==========================================
            //// xrw(bool, last_dma_tx, )
            //// xrw(bool, last_dma_rx, )

            // valid value in range [4, 16]
            xrwx(u32, bit_width, 
                (xget(ds) + 1),
                (xset(ds, value - 1))
            )

            xrw(spi_fifo_threshold, fifo_rx_threshold, frxth)
            xrw(bool, enable_fifo_tx_empty_interrupt, txeie)
            xrw(bool, enable_error_interrupt, errie)
            xrw(spi_frame_format, format, frf)

            // is invalid under format is ti_mode or phase is 1
            xrw(bool, enable_nss_pulse, nssp)

            // is invalid under format is ti_mode
            xrw(bool, enable_ss_output, ssoe)

            xrw(bool, enable_dma_tx, txdmaen)
            xrw(bool, enable_dma_rx, rxdmaen)

            xro(spi_fifo_level, fifo_tx_level, ftlvl)
            xro(spi_fifo_level, fifo_rx_level, frlvl)
            xro(bool, frame_error, fre)
            xro(bool, busy, bsy)
            xro(bool, overrun, ovr)
            xro(bool, crc_error, crcerr)
            xro(bool, underrun, udr)
            xro(bool, fifo_tx_empty, txe)
            xro(bool, fifo_rx_not_empty, rxne)

            xrwx(u08, io8, 
                (xget8(dr8)),
                (xset8(dr8, value))
            )

            xrwx(u16, io16, 
                (xget16(dr16)),
                (xset16(dr16, value))
            )

            xrw(u16, crc_poly, crcpr)
            xrw(u16, crc_rx, rxcrcr)
            xrw(u16, crc_tx, rxcrcr)
        };


        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wunused-variable"
            inline static _spi<0x40013000> spi1;
            inline static _spi<0x40003800> spi2;
            inline static _spi<0x40003c00> spi3;
            inline static _spi<0x40013400> spi4;
            inline static _spi<0x40015000> spi5;
            inline static _spi<0x40015400> spi6;
        #pragma GCC diagnostic pop
    }
#endif

namespace xuser::inc{
    using namespace mixc::extern_stm32_spi::origin;
}
