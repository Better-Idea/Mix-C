#pragma once
#include"define/base_type.hpp"
#include"macro/xlink.hpp"
#define xafio(name,...)     struct : afio_base { enum{ __VA_ARGS__ }; } constexpr name

namespace mixc{
    struct afio_base{};

    xafio(a0,
        io                   = 0x0,
        tim2_ch1             = 0x1,
        tim2_etr             = 0x1,
        tim5_ch1             = 0x2,
        tim8_etr             = 0x3,
        usart2_cts           = 0x7,
        uart4_tx             = 0x8,
        sai2_sd_b            = 0xa,
        eth_mii_crs          = 0xb,
        eventout             = 0xf,
    );

    xafio(a1,
        io                   = 0x100,
        tim2_ch2             = 0x101,
        tim5_ch2             = 0x102,
        usart2_rts           = 0x107,
        uart4_rx             = 0x108,
        quadspi_bk1_io3      = 0x109,
        sai2_mck_b           = 0x10a,
        eth_mii_rx_clk       = 0x10b,
        eth_rmii_ref_clk     = 0x10b,
        lcd_r2               = 0x10e,
        eventout             = 0x10f,
    );

    xafio(a2,
        io                   = 0x200,
        tim2_ch3             = 0x201,
        tim5_ch3             = 0x202,
        tim9_ch1             = 0x203,
        usart2_tx            = 0x207,
        sai2_sck_b           = 0x208,
        eth_mdio             = 0x20b,
        lcd_r1               = 0x20e,
        eventout             = 0x20f,
    );

    xafio(a3,
        io                   = 0x300,
        tim2_ch4             = 0x301,
        tim5_ch4             = 0x302,
        tim9_ch2             = 0x303,
        usart2_rx            = 0x307,
        otg_hs_ulpi_d0       = 0x30a,
        eth_mii_col          = 0x30b,
        lcd_b5               = 0x30e,
        eventout             = 0x30f,
    );

    xafio(a4,
        io                   = 0x400,
        spi1_nss             = 0x405,
        i2s1_ws              = 0x405,
        spi3_nss             = 0x406,
        i2s3_ws              = 0x406,
        usart2_ck            = 0x407,
        otg_hs_sof           = 0x40c,
        dcmi_hsync           = 0x40d,
        lcd_vsync            = 0x40e,
        eventout             = 0x40f,
    );

    xafio(a5,
        io                   = 0x500,
        tim2_ch1             = 0x501,
        tim2_etr             = 0x501,
        tim8_ch1n            = 0x503,
        spi1_sck             = 0x505,
        i2s1_ck              = 0x505,
        otg_hs_ulpi_ck       = 0x50a,
        lcd_r4               = 0x50e,
        eventout             = 0x50f,
    );

    xafio(a6,
        io                   = 0x600,
        tim1_bkin            = 0x601,
        tim3_ch1             = 0x602,
        tim8_bkin            = 0x603,
        spi1_miso            = 0x605,
        tim13_ch1            = 0x609,
        dcmi_pixclk          = 0x60d,
        lcd_g2               = 0x60e,
        eventout             = 0x60f,
    );

    xafio(a7,
        io                   = 0x700,
        tim1_ch1n            = 0x701,
        tim3_ch2             = 0x702,
        tim8_ch1n            = 0x703,
        spi1_mosi            = 0x705,
        i2s1_sd              = 0x705,
        tim14_ch1            = 0x709,
        eth_mii_rx_dv        = 0x70b,
        eth_rmii_crs_dv      = 0x70b,
        fmc_sdnwe            = 0x70c,
        eventout             = 0x70f,
    );

    xafio(a8,
        io                   = 0x800,
        tim1_ch1             = 0x801,
        tim8_bkin2           = 0x803,
        i2c3_scl             = 0x804,
        usart1_ck            = 0x807,
        otg_fs_sof           = 0x80a,
        lcd_r6               = 0x80e,
        eventout             = 0x80f,
    );

    xafio(a9,
        io                   = 0x900,
        tim1_ch2             = 0x901,
        i2c3_smba            = 0x904,
        spi2_sck             = 0x905,
        i2s2_ck              = 0x905,
        usart1_tx            = 0x907,
        dcmi_d0              = 0x90d,
        eventout             = 0x90f,
    );

    xafio(a10,
        io                   = 0xa00,
        tim1_ch3             = 0xa01,
        usart1_rx            = 0xa07,
        otg_fs_id            = 0xa0a,
        dcmi_d1              = 0xa0d,
        eventout             = 0xa0f,
    );

    xafio(a11,
        io                   = 0xb00,
        tim1_ch4             = 0xb01,
        usart1_cts           = 0xb07,
        can1_rx              = 0xb09,
        otg_fs_dm            = 0xb0a,
        lcd_r4               = 0xb0e,
        eventout             = 0xb0f,
    );

    xafio(a12,
        io                   = 0xc00,
        tim1_etr             = 0xc01,
        usart1_rts           = 0xc07,
        sai2_fs_b            = 0xc08,
        can1_tx              = 0xc09,
        otg_fs_dp            = 0xc0a,
        lcd_r5               = 0xc0e,
        eventout             = 0xc0f,
    );

    xafio(a13,
        io                   = 0xd00,
        eventout             = 0xd0f,
    );

    xafio(a14,
        io                   = 0xe00,
        eventout             = 0xe0f,
    );

    xafio(a15,
        io                   = 0xf00,
        tim2_ch1             = 0xf01,
        tim2_etr             = 0xf01,
        hdmicec              = 0xf04,
        spi1_nss             = 0xf05,
        i2s1_ws              = 0xf05,
        spi3_nss             = 0xf06,
        i2s3_ws              = 0xf06,
        uart4_rts            = 0xf08,
        eventout             = 0xf0f,
    );

    xafio(b0,
        io                   = 0x10000,
        tim1_ch2n            = 0x10001,
        tim3_ch3             = 0x10002,
        tim8_ch2n            = 0x10003,
        uart4_cts            = 0x10008,
        lcd_r3               = 0x10009,
        otg_hs_ulpi_d1       = 0x1000a,
        eth_mii_rxd2         = 0x1000b,
        eventout             = 0x1000f,
    );

    xafio(b1,
        io                   = 0x10100,
        tim1_ch3n            = 0x10101,
        tim3_ch4             = 0x10102,
        tim8_ch3n            = 0x10103,
        lcd_r6               = 0x10109,
        otg_hs_ulpi_d2       = 0x1010a,
        eth_mii_rxd3         = 0x1010b,
        eventout             = 0x1010f,
    );

    xafio(b2,
        io                   = 0x10200,
        sai1_sd_a            = 0x10206,
        spi3_mosi            = 0x10207,
        i2s3_sd              = 0x10207,
        quadspi_clk          = 0x10209,
        eventout             = 0x1020f,
    );

    xafio(b3,
        io                   = 0x10300,
        tim2_ch2             = 0x10301,
        spi1_sck             = 0x10305,
        i2s1_ck              = 0x10305,
        spi3_sck             = 0x10306,
        i2s3_ck              = 0x10306,
        eventout             = 0x1030f,
    );

    xafio(b4,
        io                   = 0x10400,
        tim3_ch1             = 0x10402,
        spi1_miso            = 0x10405,
        spi3_miso            = 0x10406,
        spi2_nss             = 0x10407,
        i2s2_ws              = 0x10407,
        eventout             = 0x1040f,
    );

    xafio(b5,
        io                   = 0x10500,
        tim3_ch2             = 0x10502,
        i2c1_smba            = 0x10504,
        spi1_mosi            = 0x10505,
        i2s1_sd              = 0x10505,
        spi3_mosi            = 0x10506,
        i2s3_sd              = 0x10506,
        can2_rx              = 0x10509,
        otg_hs_ulpi_d7       = 0x1050a,
        eth_pps_out          = 0x1050b,
        fmc_sdcke1           = 0x1050c,
        dcmi_d10             = 0x1050d,
        eventout             = 0x1050f,
    );

    xafio(b6,
        io                   = 0x10600,
        tim4_ch1             = 0x10602,
        hdmicec              = 0x10603,
        i2c1_scl             = 0x10604,
        usart1_tx            = 0x10607,
        can2_tx              = 0x10609,
        quadspi_bk1_ncs      = 0x1060a,
        fmc_sdne1            = 0x1060c,
        dcmi_d5              = 0x1060d,
        eventout             = 0x1060f,
    );

    xafio(b7,
        io                   = 0x10700,
        tim4_ch2             = 0x10702,
        i2c1_sda             = 0x10704,
        usart1_rx            = 0x10707,
        fmc_nl               = 0x1070c,
        dcmi_vsync           = 0x1070d,
        eventout             = 0x1070f,
    );

    xafio(b8,
        io                   = 0x10800,
        tim4_ch3             = 0x10802,
        tim10_ch1            = 0x10803,
        i2c1_scl             = 0x10804,
        can1_rx              = 0x10809,
        eth_mii_txd3         = 0x1080b,
        sdmmc1_d4            = 0x1080c,
        dcmi_d6              = 0x1080d,
        lcd_b6               = 0x1080e,
        eventout             = 0x1080f,
    );

    xafio(b9,
        io                   = 0x10900,
        tim4_ch4             = 0x10902,
        tim11_ch1            = 0x10903,
        i2c1_sda             = 0x10904,
        spi2_nss             = 0x10905,
        i2s2_ws              = 0x10905,
        can1_tx              = 0x10909,
        sdmmc1_d5            = 0x1090c,
        dcmi_d7              = 0x1090d,
        lcd_b7               = 0x1090e,
        eventout             = 0x1090f,
    );

    xafio(b10,
        io                   = 0x10a00,
        tim2_ch3             = 0x10a01,
        i2c2_scl             = 0x10a04,
        spi2_sck             = 0x10a05,
        i2s2_ck              = 0x10a05,
        usart3_tx            = 0x10a07,
        otg_hs_ulpi_d3       = 0x10a0a,
        eth_mii_rx_er        = 0x10a0b,
        lcd_g4               = 0x10a0e,
        eventout             = 0x10a0f,
    );

    xafio(b11,
        io                   = 0x10b00,
        tim2_ch4             = 0x10b01,
        i2c2_sda             = 0x10b04,
        usart3_rx            = 0x10b07,
        otg_hs_ulpi_d4       = 0x10b0a,
        eth_mii_tx_en        = 0x10b0b,
        eth_rmii_tx_en       = 0x10b0b,
        lcd_g5               = 0x10b0e,
        eventout             = 0x10b0f,
    );

    xafio(b12,
        io                   = 0x10c00,
        tim1_bkin            = 0x10c01,
        i2c2_smba            = 0x10c04,
        spi2_nss             = 0x10c05,
        i2s2_ws              = 0x10c05,
        usart3_ck            = 0x10c07,
        can2_rx              = 0x10c09,
        otg_hs_ulpi_d5       = 0x10c0a,
        eth_mii_txd0         = 0x10c0b,
        eth_rmii_txd0        = 0x10c0b,
        otg_hs_id            = 0x10c0c,
        eventout             = 0x10c0f,
    );

    xafio(b13,
        io                   = 0x10d00,
        tim1_ch1n            = 0x10d01,
        spi2_sck             = 0x10d05,
        i2s2_ck              = 0x10d05,
        usart3_cts           = 0x10d07,
        can2_tx              = 0x10d09,
        otg_hs_ulpi_d6       = 0x10d0a,
        eth_mii_txd1         = 0x10d0b,
        eth_rmii_txd1        = 0x10d0b,
        eventout             = 0x10d0f,
    );

    xafio(b14,
        io                   = 0x10e00,
        tim1_ch2n            = 0x10e01,
        tim8_ch2n            = 0x10e03,
        spi2_miso            = 0x10e05,
        usart3_rts           = 0x10e07,
        tim12_ch1            = 0x10e09,
        otg_hs_dm            = 0x10e0c,
        eventout             = 0x10e0f,
    );

    xafio(b15,
        io                   = 0x10f00,
        tim1_ch3n            = 0x10f01,
        tim8_ch3n            = 0x10f03,
        spi2_mosi            = 0x10f05,
        i2s2_sd              = 0x10f05,
        tim12_ch2            = 0x10f09,
        otg_hs_dp            = 0x10f0c,
        eventout             = 0x10f0f,
    );

    xafio(c0,
        io                   = 0x20000,
        sai2_fs_b            = 0x20008,
        otg_hs_ulpi_stp      = 0x2000a,
        fmc_sdnwe            = 0x2000c,
        lcd_r5               = 0x2000e,
        eventout             = 0x2000f,
    );

    xafio(c1,
        io                   = 0x20100,
        spi2_mosi            = 0x20105,
        i2s2_sd              = 0x20105,
        sai1_sd_a            = 0x20106,
        eth_mdc              = 0x2010b,
        eventout             = 0x2010f,
    );

    xafio(c2,
        io                   = 0x20200,
        spi2_miso            = 0x20205,
        otg_hs_ulpi_dir      = 0x2020a,
        eth_mii_txd2         = 0x2020b,
        fmc_sdne0            = 0x2020c,
        eventout             = 0x2020f,
    );

    xafio(c3,
        io                   = 0x20300,
        spi2_mosi            = 0x20305,
        i2s2_sd              = 0x20305,
        otg_hs_ulpi_nxt      = 0x2030a,
        eth_mii_tx_clk       = 0x2030b,
        fmc_sdcke0           = 0x2030c,
        eventout             = 0x2030f,
    );

    xafio(c4,
        io                   = 0x20400,
        i2s1_mck             = 0x20405,
        spdifrx_in2          = 0x20408,
        eth_mii_rxd0         = 0x2040b,
        eth_rmii_rxd0        = 0x2040b,
        fmc_sdne0            = 0x2040c,
        eventout             = 0x2040f,
    );

    xafio(c5,
        io                   = 0x20500,
        spdifrx_in3          = 0x20508,
        eth_mii_rxd1         = 0x2050b,
        eth_rmii_rxd1        = 0x2050b,
        fmc_sdcke0           = 0x2050c,
        eventout             = 0x2050f,
    );

    xafio(c6,
        io                   = 0x20600,
        tim3_ch1             = 0x20602,
        tim8_ch1             = 0x20603,
        i2s2_mck             = 0x20605,
        usart6_tx            = 0x20608,
        sdmmc1_d6            = 0x2060c,
        dcmi_d0              = 0x2060d,
        lcd_hsync            = 0x2060e,
        eventout             = 0x2060f,
    );

    xafio(c7,
        io                   = 0x20700,
        tim3_ch2             = 0x20702,
        tim8_ch2             = 0x20703,
        i2s3_mck             = 0x20706,
        usart6_rx            = 0x20708,
        sdmmc1_d7            = 0x2070c,
        dcmi_d1              = 0x2070d,
        lcd_g6               = 0x2070e,
        eventout             = 0x2070f,
    );

    xafio(c8,
        io                   = 0x20800,
        tim3_ch3             = 0x20802,
        tim8_ch3             = 0x20803,
        uart5_rts            = 0x20807,
        usart6_ck            = 0x20808,
        sdmmc1_d0            = 0x2080c,
        dcmi_d2              = 0x2080d,
        eventout             = 0x2080f,
    );

    xafio(c9,
        io                   = 0x20900,
        tim3_ch4             = 0x20902,
        tim8_ch4             = 0x20903,
        i2c3_sda             = 0x20904,
        i2s_ckin             = 0x20905,
        uart5_cts            = 0x20907,
        quadspi_bk1_io0      = 0x20909,
        sdmmc1_d1            = 0x2090c,
        dcmi_d3              = 0x2090d,
        eventout             = 0x2090f,
    );

    xafio(c10,
        io                   = 0x20a00,
        spi3_sck             = 0x20a06,
        i2s3_ck              = 0x20a06,
        usart3_tx            = 0x20a07,
        uart4_tx             = 0x20a08,
        quadspi_bk1_io1      = 0x20a09,
        sdmmc1_d2            = 0x20a0c,
        dcmi_d8              = 0x20a0d,
        lcd_r2               = 0x20a0e,
        eventout             = 0x20a0f,
    );

    xafio(c11,
        io                   = 0x20b00,
        spi3_miso            = 0x20b06,
        usart3_rx            = 0x20b07,
        uart4_rx             = 0x20b08,
        quadspi_bk2_ncs      = 0x20b09,
        sdmmc1_d3            = 0x20b0c,
        dcmi_d4              = 0x20b0d,
        eventout             = 0x20b0f,
    );

    xafio(c12,
        io                   = 0x20c00,
        spi3_mosi            = 0x20c06,
        i2s3_sd              = 0x20c06,
        usart3_ck            = 0x20c07,
        uart5_tx             = 0x20c08,
        sdmmc1_ck            = 0x20c0c,
        dcmi_d9              = 0x20c0d,
        eventout             = 0x20c0f,
    );

    xafio(c13,
        io                   = 0x20d00,
        eventout             = 0x20d0f,
    );

    xafio(c14,
        io                   = 0x20e00,
        eventout             = 0x20e0f,
    );

    xafio(c15,
        io                   = 0x20f00,
        eventout             = 0x20f0f,
    );

    xafio(d0,
        io                   = 0x30000,
        can1_rx              = 0x30009,
        fmc_d2               = 0x3000c,
        eventout             = 0x3000f,
    );

    xafio(d1,
        io                   = 0x30100,
        can1_tx              = 0x30109,
        fmc_d3               = 0x3010c,
        eventout             = 0x3010f,
    );

    xafio(d2,
        io                   = 0x30200,
        tim3_etr             = 0x30202,
        uart5_rx             = 0x30208,
        sdmmc1_cmd           = 0x3020c,
        dcmi_d11             = 0x3020d,
        eventout             = 0x3020f,
    );

    xafio(d3,
        io                   = 0x30300,
        spi2_sck             = 0x30305,
        i2s2_ck              = 0x30305,
        usart2_cts           = 0x30307,
        fmc_clk              = 0x3030c,
        dcmi_d5              = 0x3030d,
        lcd_g7               = 0x3030e,
        eventout             = 0x3030f,
    );

    xafio(d4,
        io                   = 0x30400,
        usart2_rts           = 0x30407,
        fmc_noe              = 0x3040c,
        eventout             = 0x3040f,
    );

    xafio(d5,
        io                   = 0x30500,
        usart2_tx            = 0x30507,
        fmc_nwe              = 0x3050c,
        eventout             = 0x3050f,
    );

    xafio(d6,
        io                   = 0x30600,
        spi3_mosi            = 0x30605,
        i2s3_sd              = 0x30605,
        sai1_sd_a            = 0x30606,
        usart2_rx            = 0x30607,
        fmc_nwait            = 0x3060c,
        dcmi_d10             = 0x3060d,
        lcd_b2               = 0x3060e,
        eventout             = 0x3060f,
    );

    xafio(d7,
        io                   = 0x30700,
        usart2_ck            = 0x30707,
        spdifrx_in0          = 0x30708,
        fmc_ne1              = 0x3070c,
        eventout             = 0x3070f,
    );

    xafio(d8,
        io                   = 0x30800,
        usart3_tx            = 0x30807,
        spdifrx_in1          = 0x30808,
        fmc_d13              = 0x3080c,
        eventout             = 0x3080f,
    );

    xafio(d9,
        io                   = 0x30900,
        usart3_rx            = 0x30907,
        fmc_d14              = 0x3090c,
        eventout             = 0x3090f,
    );

    xafio(d10,
        io                   = 0x30a00,
        usart3_ck            = 0x30a07,
        fmc_d15              = 0x30a0c,
        lcd_b3               = 0x30a0e,
        eventout             = 0x30a0f,
    );

    xafio(d11,
        io                   = 0x30b00,
        i2c4_smba            = 0x30b04,
        usart3_cts           = 0x30b07,
        quadspi_bk1_io0      = 0x30b09,
        sai2_sd_a            = 0x30b0a,
        fmc_a16              = 0x30b0c,
        fmc_cle              = 0x30b0c,
        eventout             = 0x30b0f,
    );

    xafio(d12,
        io                   = 0x30c00,
        tim4_ch1             = 0x30c02,
        lptim1_in1           = 0x30c03,
        i2c4_scl             = 0x30c04,
        usart3_rts           = 0x30c07,
        quadspi_bk1_io1      = 0x30c09,
        sai2_fs_a            = 0x30c0a,
        fmc_a17              = 0x30c0c,
        fmc_ale              = 0x30c0c,
        eventout             = 0x30c0f,
    );

    xafio(d13,
        io                   = 0x30d00,
        tim4_ch2             = 0x30d02,
        lptim1_out           = 0x30d03,
        i2c4_sda             = 0x30d04,
        quadspi_bk1_io3      = 0x30d09,
        sai2_sck_a           = 0x30d0a,
        fmc_a18              = 0x30d0c,
        eventout             = 0x30d0f,
    );

    xafio(d14,
        io                   = 0x30e00,
        tim4_ch3             = 0x30e02,
        uart8_cts            = 0x30e08,
        fmc_d0               = 0x30e0c,
        eventout             = 0x30e0f,
    );

    xafio(d15,
        io                   = 0x30f00,
        tim4_ch4             = 0x30f02,
        uart8_rts            = 0x30f08,
        fmc_d1               = 0x30f0c,
        eventout             = 0x30f0f,
    );

    xafio(e0,
        io                   = 0x40000,
        tim4_etr             = 0x40002,
        lptim1_etr           = 0x40003,
        uart8_rx             = 0x40008,
        sai2_mck_a           = 0x4000a,
        fmc_nbl0             = 0x4000c,
        dcmi_d2              = 0x4000d,
        eventout             = 0x4000f,
    );

    xafio(e1,
        io                   = 0x40100,
        lptim1_in2           = 0x40103,
        uart8_tx             = 0x40108,
        fmc_nbl1             = 0x4010c,
        dcmi_d3              = 0x4010d,
        eventout             = 0x4010f,
    );

    xafio(e2,
        io                   = 0x40200,
        spi4_sck             = 0x40205,
        sai1_mclk_a          = 0x40206,
        quadspi_bk1_io2      = 0x40209,
        eth_mii_txd3         = 0x4020b,
        fmc_a23              = 0x4020c,
        eventout             = 0x4020f,
    );

    xafio(e3,
        io                   = 0x40300,
        sai1_sd_b            = 0x40306,
        fmc_a19              = 0x4030c,
        eventout             = 0x4030f,
    );

    xafio(e4,
        io                   = 0x40400,
        spi4_nss             = 0x40405,
        sai1_fs_a            = 0x40406,
        fmc_a20              = 0x4040c,
        dcmi_d4              = 0x4040d,
        lcd_b0               = 0x4040e,
        eventout             = 0x4040f,
    );

    xafio(e5,
        io                   = 0x40500,
        tim9_ch1             = 0x40503,
        spi4_miso            = 0x40505,
        sai1_sck_a           = 0x40506,
        fmc_a21              = 0x4050c,
        dcmi_d6              = 0x4050d,
        lcd_g0               = 0x4050e,
        eventout             = 0x4050f,
    );

    xafio(e6,
        io                   = 0x40600,
        tim1_bkin2           = 0x40601,
        tim9_ch2             = 0x40603,
        spi4_mosi            = 0x40605,
        sai1_sd_a            = 0x40606,
        sai2_mck_b           = 0x4060a,
        fmc_a22              = 0x4060c,
        dcmi_d7              = 0x4060d,
        lcd_g1               = 0x4060e,
        eventout             = 0x4060f,
    );

    xafio(e7,
        io                   = 0x40700,
        tim1_etr             = 0x40701,
        uart7_rx             = 0x40708,
        quadspi_bk2_io0      = 0x4070a,
        fmc_d4               = 0x4070c,
        eventout             = 0x4070f,
    );

    xafio(e8,
        io                   = 0x40800,
        tim1_ch1n            = 0x40801,
        uart7_tx             = 0x40808,
        quadspi_bk2_io1      = 0x4080a,
        fmc_d5               = 0x4080c,
        eventout             = 0x4080f,
    );

    xafio(e9,
        io                   = 0x40900,
        tim1_ch1             = 0x40901,
        uart7_rts            = 0x40908,
        quadspi_bk2_io2      = 0x4090a,
        fmc_d6               = 0x4090c,
        eventout             = 0x4090f,
    );

    xafio(e10,
        io                   = 0x40a00,
        tim1_ch2n            = 0x40a01,
        uart7_cts            = 0x40a08,
        quadspi_bk2_io3      = 0x40a0a,
        fmc_d7               = 0x40a0c,
        eventout             = 0x40a0f,
    );

    xafio(e11,
        io                   = 0x40b00,
        tim1_ch2             = 0x40b01,
        spi4_nss             = 0x40b05,
        sai2_sd_b            = 0x40b0a,
        fmc_d8               = 0x40b0c,
        lcd_g3               = 0x40b0e,
        eventout             = 0x40b0f,
    );

    xafio(e12,
        io                   = 0x40c00,
        tim1_ch3n            = 0x40c01,
        spi4_sck             = 0x40c05,
        sai2_sck_b           = 0x40c0a,
        fmc_d9               = 0x40c0c,
        lcd_b4               = 0x40c0e,
        eventout             = 0x40c0f,
    );

    xafio(e13,
        io                   = 0x40d00,
        tim1_ch3             = 0x40d01,
        spi4_miso            = 0x40d05,
        sai2_fs_b            = 0x40d0a,
        fmc_d10              = 0x40d0c,
        lcd_de               = 0x40d0e,
        eventout             = 0x40d0f,
    );

    xafio(e14,
        io                   = 0x40e00,
        tim1_ch4             = 0x40e01,
        spi4_mosi            = 0x40e05,
        sai2_mck_b           = 0x40e0a,
        fmc_d11              = 0x40e0c,
        lcd_clk              = 0x40e0e,
        eventout             = 0x40e0f,
    );

    xafio(e15,
        io                   = 0x40f00,
        tim1_bkin            = 0x40f01,
        fmc_d12              = 0x40f0c,
        lcd_r7               = 0x40f0e,
        eventout             = 0x40f0f,
    );

    xafio(f0,
        io                   = 0x50000,
        i2c2_sda             = 0x50004,
        fmc_a0               = 0x5000c,
        eventout             = 0x5000f,
    );

    xafio(f1,
        io                   = 0x50100,
        i2c2_scl             = 0x50104,
        fmc_a1               = 0x5010c,
        eventout             = 0x5010f,
    );

    xafio(f2,
        io                   = 0x50200,
        i2c2_smba            = 0x50204,
        fmc_a2               = 0x5020c,
        eventout             = 0x5020f,
    );

    xafio(f3,
        io                   = 0x50300,
        fmc_a3               = 0x5030c,
        eventout             = 0x5030f,
    );

    xafio(f4,
        io                   = 0x50400,
        fmc_a4               = 0x5040c,
        eventout             = 0x5040f,
    );

    xafio(f5,
        io                   = 0x50500,
        fmc_a5               = 0x5050c,
        eventout             = 0x5050f,
    );

    xafio(f6,
        io                   = 0x50600,
        tim10_ch1            = 0x50603,
        spi5_nss             = 0x50605,
        sai1_sd_b            = 0x50606,
        uart7_rx             = 0x50608,
        quadspi_bk1_io3      = 0x50609,
        eventout             = 0x5060f,
    );

    xafio(f7,
        io                   = 0x50700,
        tim11_ch1            = 0x50703,
        spi5_sck             = 0x50705,
        sai1_mclk_b          = 0x50706,
        uart7_tx             = 0x50708,
        quadspi_bk1_io2      = 0x50709,
        eventout             = 0x5070f,
    );

    xafio(f8,
        io                   = 0x50800,
        spi5_miso            = 0x50805,
        sai1_sck_b           = 0x50806,
        uart7_rts            = 0x50808,
        tim13_ch1            = 0x50809,
        quadspi_bk1_io0      = 0x5080a,
        eventout             = 0x5080f,
    );

    xafio(f9,
        io                   = 0x50900,
        spi5_mosi            = 0x50905,
        sai1_fs_b            = 0x50906,
        uart7_cts            = 0x50908,
        tim14_ch1            = 0x50909,
        quadspi_bk1_io1      = 0x5090a,
        eventout             = 0x5090f,
    );

    xafio(f10,
        io                   = 0x50a00,
        dcmi_d11             = 0x50a0d,
        lcd_de               = 0x50a0e,
        eventout             = 0x50a0f,
    );

    xafio(f11,
        io                   = 0x50b00,
        spi5_mosi            = 0x50b05,
        sai2_sd_b            = 0x50b0a,
        fmc_sdnras           = 0x50b0c,
        dcmi_d12             = 0x50b0d,
        eventout             = 0x50b0f,
    );

    xafio(f12,
        io                   = 0x50c00,
        fmc_a6               = 0x50c0c,
        eventout             = 0x50c0f,
    );

    xafio(f13,
        io                   = 0x50d00,
        i2c4_smba            = 0x50d04,
        fmc_a7               = 0x50d0c,
        eventout             = 0x50d0f,
    );

    xafio(f14,
        io                   = 0x50e00,
        i2c4_scl             = 0x50e04,
        fmc_a8               = 0x50e0c,
        eventout             = 0x50e0f,
    );

    xafio(f15,
        io                   = 0x50f00,
        i2c4_sda             = 0x50f04,
        fmc_a9               = 0x50f0c,
        eventout             = 0x50f0f,
    );

    xafio(g0,
        io                   = 0x60000,
        fmc_a10              = 0x6000c,
        eventout             = 0x6000f,
    );

    xafio(g1,
        io                   = 0x60100,
        fmc_a11              = 0x6010c,
        eventout             = 0x6010f,
    );

    xafio(g2,
        io                   = 0x60200,
        fmc_a12              = 0x6020c,
        eventout             = 0x6020f,
    );

    xafio(g3,
        io                   = 0x60300,
        fmc_a13              = 0x6030c,
        eventout             = 0x6030f,
    );

    xafio(g4,
        io                   = 0x60400,
        fmc_a14              = 0x6040c,
        fmc_ba0              = 0x6040c,
        eventout             = 0x6040f,
    );

    xafio(g5,
        io                   = 0x60500,
        fmc_a15              = 0x6050c,
        fmc_ba1              = 0x6050c,
        eventout             = 0x6050f,
    );

    xafio(g6,
        io                   = 0x60600,
        dcmi_d12             = 0x6060d,
        lcd_r7               = 0x6060e,
        eventout             = 0x6060f,
    );

    xafio(g7,
        io                   = 0x60700,
        usart6_ck            = 0x60708,
        fmc_int              = 0x6070c,
        dcmi_d13             = 0x6070d,
        lcd_clk              = 0x6070e,
        eventout             = 0x6070f,
    );

    xafio(g8,
        io                   = 0x60800,
        spi6_nss             = 0x60805,
        spdifrx_in2          = 0x60807,
        usart6_rts           = 0x60808,
        eth_pps_out          = 0x6080b,
        fmc_sdclk            = 0x6080c,
        eventout             = 0x6080f,
    );

    xafio(g9,
        io                   = 0x60900,
        spdifrx_in3          = 0x60907,
        usart6_rx            = 0x60908,
        quadspi_bk2_io2      = 0x60909,
        sai2_fs_b            = 0x6090a,
        fmc_ne2              = 0x6090c,
        fmc_nce              = 0x6090c,
        dcmi_vsync           = 0x6090d,
        eventout             = 0x6090f,
    );

    xafio(g10,
        io                   = 0x60a00,
        lcd_g3               = 0x60a09,
        sai2_sd_b            = 0x60a0a,
        fmc_ne3              = 0x60a0c,
        dcmi_d2              = 0x60a0d,
        lcd_b2               = 0x60a0e,
        eventout             = 0x60a0f,
    );

    xafio(g11,
        io                   = 0x60b00,
        spdifrx_in0          = 0x60b07,
        eth_mii_tx_en        = 0x60b0b,
        eth_rmii_tx_en       = 0x60b0b,
        dcmi_d3              = 0x60b0d,
        lcd_b3               = 0x60b0e,
        eventout             = 0x60b0f,
    );

    xafio(g12,
        io                   = 0x60c00,
        lptim1_in1           = 0x60c03,
        spi6_miso            = 0x60c05,
        spdifrx_in1          = 0x60c07,
        usart6_rts           = 0x60c08,
        lcd_b4               = 0x60c09,
        fmc_ne4              = 0x60c0c,
        lcd_b1               = 0x60c0e,
        eventout             = 0x60c0f,
    );

    xafio(g13,
        io                   = 0x60d00,
        lptim1_out           = 0x60d03,
        spi6_sck             = 0x60d05,
        usart6_cts           = 0x60d08,
        eth_mii_txd0         = 0x60d0b,
        eth_rmii_txd0        = 0x60d0b,
        fmc_a24              = 0x60d0c,
        lcd_r0               = 0x60d0e,
        eventout             = 0x60d0f,
    );

    xafio(g14,
        io                   = 0x60e00,
        lptim1_etr           = 0x60e03,
        spi6_mosi            = 0x60e05,
        usart6_tx            = 0x60e08,
        quadspi_bk2_io3      = 0x60e09,
        eth_mii_txd1         = 0x60e0b,
        eth_rmii_txd1        = 0x60e0b,
        fmc_a25              = 0x60e0c,
        lcd_b0               = 0x60e0e,
        eventout             = 0x60e0f,
    );

    xafio(g15,
        io                   = 0x60f00,
        usart6_cts           = 0x60f08,
        fmc_sdncas           = 0x60f0c,
        dcmi_d13             = 0x60f0d,
        eventout             = 0x60f0f,
    );

    xafio(h0,
        io                   = 0x70000,
        eventout             = 0x7000f,
    );

    xafio(h1,
        io                   = 0x70100,
        eventout             = 0x7010f,
    );

    xafio(h2,
        io                   = 0x70200,
        lptim1_in2           = 0x70203,
        quadspi_bk2_io0      = 0x70209,
        sai2_sck_b           = 0x7020a,
        eth_mii_crs          = 0x7020b,
        fmc_sdcke0           = 0x7020c,
        lcd_r0               = 0x7020e,
        eventout             = 0x7020f,
    );

    xafio(h3,
        io                   = 0x70300,
        quadspi_bk2_io1      = 0x70309,
        sai2_mck_b           = 0x7030a,
        eth_mii_col          = 0x7030b,
        fmc_sdne0            = 0x7030c,
        lcd_r1               = 0x7030e,
        eventout             = 0x7030f,
    );

    xafio(h4,
        io                   = 0x70400,
        i2c2_scl             = 0x70404,
        otg_hs_ulpi_nxt      = 0x7040a,
        eventout             = 0x7040f,
    );

    xafio(h5,
        io                   = 0x70500,
        i2c2_sda             = 0x70504,
        spi5_nss             = 0x70505,
        fmc_sdnwe            = 0x7050c,
        eventout             = 0x7050f,
    );

    xafio(h6,
        io                   = 0x70600,
        i2c2_smba            = 0x70604,
        spi5_sck             = 0x70605,
        tim12_ch1            = 0x70609,
        eth_mii_rxd2         = 0x7060b,
        fmc_sdne1            = 0x7060c,
        dcmi_d8              = 0x7060d,
        eventout             = 0x7060f,
    );

    xafio(h7,
        io                   = 0x70700,
        i2c3_scl             = 0x70704,
        spi5_miso            = 0x70705,
        eth_mii_rxd3         = 0x7070b,
        fmc_sdcke1           = 0x7070c,
        dcmi_d9              = 0x7070d,
        eventout             = 0x7070f,
    );

    xafio(h8,
        io                   = 0x70800,
        i2c3_sda             = 0x70804,
        fmc_d16              = 0x7080c,
        dcmi_hsync           = 0x7080d,
        lcd_r2               = 0x7080e,
        eventout             = 0x7080f,
    );

    xafio(h9,
        io                   = 0x70900,
        i2c3_smba            = 0x70904,
        tim12_ch2            = 0x70909,
        fmc_d17              = 0x7090c,
        dcmi_d0              = 0x7090d,
        lcd_r3               = 0x7090e,
        eventout             = 0x7090f,
    );

    xafio(h10,
        io                   = 0x70a00,
        tim5_ch1             = 0x70a02,
        i2c4_smba            = 0x70a04,
        fmc_d18              = 0x70a0c,
        dcmi_d1              = 0x70a0d,
        lcd_r4               = 0x70a0e,
        eventout             = 0x70a0f,
    );

    xafio(h11,
        io                   = 0x70b00,
        tim5_ch2             = 0x70b02,
        i2c4_scl             = 0x70b04,
        fmc_d19              = 0x70b0c,
        dcmi_d2              = 0x70b0d,
        lcd_r5               = 0x70b0e,
        eventout             = 0x70b0f,
    );

    xafio(h12,
        io                   = 0x70c00,
        tim5_ch3             = 0x70c02,
        i2c4_sda             = 0x70c04,
        fmc_d20              = 0x70c0c,
        dcmi_d3              = 0x70c0d,
        lcd_r6               = 0x70c0e,
        eventout             = 0x70c0f,
    );

    xafio(h13,
        io                   = 0x70d00,
        tim8_ch1n            = 0x70d03,
        can1_tx              = 0x70d09,
        fmc_d21              = 0x70d0c,
        lcd_g2               = 0x70d0e,
        eventout             = 0x70d0f,
    );

    xafio(h14,
        io                   = 0x70e00,
        tim8_ch2n            = 0x70e03,
        fmc_d22              = 0x70e0c,
        dcmi_d4              = 0x70e0d,
        lcd_g3               = 0x70e0e,
        eventout             = 0x70e0f,
    );

    xafio(h15,
        io                   = 0x70f00,
        tim8_ch3n            = 0x70f03,
        fmc_d23              = 0x70f0c,
        dcmi_d11             = 0x70f0d,
        lcd_g4               = 0x70f0e,
        eventout             = 0x70f0f,
    );

    xafio(i0,
        io                   = 0x80000,
        tim5_ch4             = 0x80002,
        spi2_nss             = 0x80005,
        i2s2_ws              = 0x80005,
        fmc_d24              = 0x8000c,
        dcmi_d13             = 0x8000d,
        lcd_g5               = 0x8000e,
        eventout             = 0x8000f,
    );

    xafio(i1,
        io                   = 0x80100,
        tim8_bkin2           = 0x80103,
        spi2_sck             = 0x80105,
        i2s2_ck              = 0x80105,
        fmc_d25              = 0x8010c,
        dcmi_d8              = 0x8010d,
        lcd_g6               = 0x8010e,
        eventout             = 0x8010f,
    );

    xafio(i2,
        io                   = 0x80200,
        tim8_ch4             = 0x80203,
        spi2_miso            = 0x80205,
        fmc_d26              = 0x8020c,
        dcmi_d9              = 0x8020d,
        lcd_g7               = 0x8020e,
        eventout             = 0x8020f,
    );

    xafio(i3,
        io                   = 0x80300,
        tim8_etr             = 0x80303,
        spi2_mosi            = 0x80305,
        i2s2_sd              = 0x80305,
        fmc_d27              = 0x8030c,
        dcmi_d10             = 0x8030d,
        eventout             = 0x8030f,
    );

    xafio(i4,
        io                   = 0x80400,
        tim8_bkin            = 0x80403,
        sai2_mck_a           = 0x8040a,
        fmc_nbl2             = 0x8040c,
        dcmi_d5              = 0x8040d,
        lcd_b4               = 0x8040e,
        eventout             = 0x8040f,
    );

    xafio(i5,
        io                   = 0x80500,
        tim8_ch1             = 0x80503,
        sai2_sck_a           = 0x8050a,
        fmc_nbl3             = 0x8050c,
        dcmi_vsync           = 0x8050d,
        lcd_b5               = 0x8050e,
        eventout             = 0x8050f,
    );

    xafio(i6,
        io                   = 0x80600,
        tim8_ch2             = 0x80603,
        sai2_sd_a            = 0x8060a,
        fmc_d28              = 0x8060c,
        dcmi_d6              = 0x8060d,
        lcd_b6               = 0x8060e,
        eventout             = 0x8060f,
    );

    xafio(i7,
        io                   = 0x80700,
        tim8_ch3             = 0x80703,
        sai2_fs_a            = 0x8070a,
        fmc_d29              = 0x8070c,
        dcmi_d7              = 0x8070d,
        lcd_b7               = 0x8070e,
        eventout             = 0x8070f,
    );

    xafio(i8,
        io                   = 0x80800,
        eventout             = 0x8080f,
    );

    xafio(i9,
        io                   = 0x80900,
        can1_rx              = 0x80909,
        fmc_d30              = 0x8090c,
        lcd_vsync            = 0x8090e,
        eventout             = 0x8090f,
    );

    xafio(i10,
        io                   = 0x80a00,
        eth_mii_rx_er        = 0x80a0b,
        fmc_d31              = 0x80a0c,
        lcd_hsync            = 0x80a0e,
        eventout             = 0x80a0f,
    );

    xafio(i11,
        io                   = 0x80b00,
        otg_hs_ulpi_dir      = 0x80b0a,
        eventout             = 0x80b0f,
    );

    xafio(i12,
        io                   = 0x80c00,
        lcd_hsync            = 0x80c0e,
        eventout             = 0x80c0f,
    );

    xafio(i13,
        io                   = 0x80d00,
        lcd_vsync            = 0x80d0e,
        eventout             = 0x80d0f,
    );

    xafio(i14,
        io                   = 0x80e00,
        lcd_clk              = 0x80e0e,
        eventout             = 0x80e0f,
    );

}

#undef  xafio