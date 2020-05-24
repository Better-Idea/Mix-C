#ifndef xpack_extern_stm32_dac
#define xpack_extern_stm32_dac
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::extern_stm32_dac
        #include"define/bitbind.hpp"
        #include"define/mfxx.hpp"
        #include"extern/stm32/define.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::extern_stm32_dac{
        enum class dac_source_for_channel_trigger{
            from_tim6,
            from_tim8,
            from_tim7,
            from_tim5,
            from_tim2,
            from_tim4,
            from_exti_line9,
            from_software,
        };

        enum class dac_ware_type{
            none_ware,
            noise_ware,
            triangle_ware,
        };

        enum class dac_ware_amplitude{
            equals_to_1, equals_to_3, equals_to_7, equals_to_15, equals_to_31, equals_to_63, equals_to_127, equals_to_255, equals_to_511, equals_to_1023, equals_to_2047, equals_to_4095,
        };

        struct not_t{
            not_t(bool value = false) : value(value){}
            operator bool(){
                return !value;
            }
            void operator = (bool value){
                this->value = !value;
            }
        private:
            bool value;
        };

        // NOTE:
        // - dac1 & dac2 combine operation register is leave out
        // - 8bit precision is leave out
        // - left align mode is leave out
        xmmio(_dac){
        private:
            u32 en1         : 1;
            u32 boff1       : 1;
            u32 ten1        : 1;
            u32 tsel1       : 3;
            u32 ware1       : 2;
            u32 mamp1       : 4;
            u32 dmaen1      : 1;
            u32 dmaudrie1   : 1;
            xr32(2);
            u32 en2         : 1;
            u32 boff2       : 1;
            u32 ten2        : 1;
            u32 tsel2       : 3;
            u32 ware2       : 2;
            u32 mamp2       : 4;
            u32 dmaen2      : 1;
            u32 dmaudrie2   : 1;
            xr32(2);

            u32 swtrig1     : 1;
            u32 swtrig2     : 1;
            xr32(30);

            u32 dhr12r1     : 12;
            xr32(20);

            xr32(4);
            u32 dhr12l1     : 12;
            xr32(16);

            u32 dhr8r1      : 8;
            xr32(24);

            u32 dhr12r2     : 12;
            xr32(20);

            xr32(4);
            u32 dhr12l2     : 12;
            xr32(16);

            u32 dhr8r2      : 8;
            xr32(24);

            u32 dhr12rd1    : 12;
            xr32(4);
            u32 dhr12rd2    : 12;
            xr32(4);

            xr32(4);
            u32 dhr12ld1    : 12;
            xr32(4);
            u32 dhr12ld2    : 12;
            
            u32 hdr8rd1     : 8;
            u32 hdr8rd2     : 8;
            xr32(16);

            u32 dor1        : 12;
            xr32(20);

            u32 dor2        : 12;
            xr32(20);

            xr32(13);
            u32 dmaudr1     : 1;
            xr32(2);
            xr32(13);
            u32 dmaudr2     : 1;
            xr32(2);

            using the_t = _dac<address>;

            static the_t & mmio(){
                return *(the_t *)address;
            }
        public:
            #define xarg 1
            #include"extern/stm32/private/gen_dac.hpp"

            #define xarg 2
            #include"extern/stm32/private/gen_dac.hpp"
            static inline f32 voltage_ref = 3.3;
        };
    }
#endif

