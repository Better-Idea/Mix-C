#include"extern/stm32/firmware/core.hpp"

xseg(.entry) xmain()
    f32 v[] = { 2.0, 2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3.0, 3.1, 3.2 };
    rcc.dac(enable);
    rcc.gpioa(enable);
    ioa.config = { analog, 4 }; // dac1 map to PA4
    dac.one.voltage = 2.0;
    dac.one.enable = true;
    xwhile(true)
        xfor(uxx i = 0; i < 11; i++)
            dac.one.voltage = v[i];
        $
    $
    return 0;
$
