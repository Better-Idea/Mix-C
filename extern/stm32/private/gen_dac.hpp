#ifndef xarg
    #error "need set the type"
#else
    union{
        using the_t = _dac;

        /* - 0 : diable dac channel */
        /* - 1 : enable dac channel */
        xrw(bool, enable, xlink2(en, xarg));

        /* - 0 : disable dac output buffer */
        /* - 1 : enable dac output buffer */
        xrw(not_t, enable_output_buffer, xlink2(boff, xarg));

        /* - 0 : wrtie data to data hold register whill move to data output register after 1 apb1 clock */
        /* - 1 : wrtie data to data hold register whill move to data output register after 3 apb1 clock */
        union{
            xrw(bool, enable, xlink2(ten, xarg));
            xrw(dac_source_for_channel_trigger, source, xlink2(tsel, xarg));
            xrw(dac_ware_type, type, xlink2(ware, xarg));
            xrw(dac_ware_amplitude, amplitude, xlink2(mamp, xarg));
        } triggle;

        union{
            xrw(bool, enable, xlink2(dmaen, xarg));
            xrw(bool, enable_underrun_interrupt, xlink2(dmaudrie, xarg));
            xr1(bool, underrun, xlink2(dmaudr, xarg))
        } dma;

        /* this bit is clear by hardware(one APB1 clock cycle later)
            once the data hold register value has been loaded into data output register */
        xrw(bool, software_triggler, xlink2(swtrig, xarg));

        /* 12bit precision */
        xrw(u32, value, xlink2(dhr12r, xarg))

        /* 12bit precision (float format) */
        xrwx(f32, voltage, 
            (f32(xlink2(the.dhr12r, xarg)) / 4096 * voltage_ref),
            (xlink2(the.dhr12r, xarg) = f32(value * 4096) / voltage_ref)
        )
    } xlink2(dac, xarg);

    #undef xarg
#endif
