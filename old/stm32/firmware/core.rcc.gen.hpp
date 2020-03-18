#ifndef xarg
    #error "need set the type"
#else
    #define xis_rst_part    (xarg == 0)
    #define xis_en_part     (xarg == 1)
    #define xis_lpen_part   (xarg == 2)
    
    #if xis_rst_part  
        #define xx(name,bits)   u32 xlink(name,rst) : bits;
    #elif xis_en_part     
        #define xx(name,bits)   u32 xlink(name,en) : bits;
    #elif xis_lpen_part  
        #define xx(name,bits)   u32 xlink(name,lpen) : bits;
    #endif

    xx(gpioa    , 1);
    xx(gpiob    , 1);
    xx(gpioc    , 1);
    xx(gpiod    , 1);
    xx(gpioe    , 1);
    xx(gpiof    , 1);
    xx(gpiog    , 1);
    xx(gpioh    , 1);
    xx(gpioi    , 1);
    xx(gpioj    , 1);
    xx(gpiok    , 1);
    xr32(1);
    xx(crc      , 1);

    #if xis_rst_part
        xr32(8);
    #else
        #if xis_en_part
            xr32(5);
        #elif xis_lpen_part
            xx(axi  , 1);
            xr32(1);
            xx(flash, 1);
            xx(sram1, 1);
            xx(sram2, 1);
        #endif
        xx(backup_sram, 1);
        xr32(1);
        xx(tdcm_ram, 1);
    #endif

    xx(dma1     , 1);
    xx(dma2     , 1);
    xx(dma2d    , 1);
    xr32(1);
    xx(eth      , 1);

    #if xis_rst_part
        xr32(3);
    #else
        xx(eth_tx , 1);
        xx(eth_rx , 1);
        xx(eth_ptp, 1);
    #endif

    xx(otghs    , 1);

    #if xis_rst_part
        xr32(2);
    #else
        xx(otghs_ulpi, 1);
        xr32(1);
    #endif

    xx(dcmi     , 1);
    xr32(3);
    xx(crypto   , 1);
    xx(hash     , 1);
    xx(random   , 1);
    xx(otgfs    , 1);
    xr32(24);

    xx(fmc      , 1);
    xx(qspi     , 1);
    xr32(30);

    xr32(32);

    xx(tim2     , 1);
    xx(tim3     , 1);
    xx(tim4     , 1);
    xx(tim5     , 1);
    xx(tim6     , 1);
    xx(tim7     , 1);
    xx(tim12    , 1);
    xx(tim13    , 1);
    xx(tim14    , 1);
    xx(lptim1   , 1);
    xr32(1);
    xx(wwdg     , 1);
    xr32(2);
    xx(spi2     , 1);
    xx(spi3     , 1);
    xx(spdif_rx , 1);
    xx(uart2    , 1);
    xx(uart3    , 1);
    xx(uart4    , 1);
    xx(uart5    , 1);
    xx(i2c1     , 1);
    xx(i2c2     , 1);
    xx(i2c3     , 1);
    xx(i2c4     , 1);
    xx(can1     , 1);
    xx(can2     , 1);
    xx(cec      , 1);
    xx(pwr      , 1);
    xx(dac      , 1);
    xx(uart7    , 1);
    xx(uart8    , 1);

    xx(tim1     , 1);
    xx(tim8     , 1);
    xr32(2);
    xx(usart1   , 1);
    xx(usart6   , 1);
    xr32(2);

    #if xis_rst_part
        xx(adc  , 1);
        xr32(2);
    #else
        xx(adc1 , 1);
        xx(adc2 , 1);
        xx(adc3 , 1);
    #endif

    xx(sdmmc1   , 1);
    xx(spi1     , 1);
    xx(spi4     , 1);
    xx(syscfg   , 1);
    xr32(1);
    xx(tim9     , 1);
    xx(tim10    , 1);
    xx(tim11    , 1);
    xr32(1);
    xx(spi5     , 1);
    xx(spi6     , 1);
    xx(sai1     , 1);
    xx(sai2     , 1);
    xr32(2);                
    xx(ltdc     , 1);
    xr32(5);

    xr32(32);
    xr32(32);

    #undef xx
    #undef xarg
    #undef xis_rst_part
    #undef xis_en_part
    #undef xis_lpen_part
    #undef xarg
#endif

