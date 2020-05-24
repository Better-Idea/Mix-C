constexpr port                  keep_variable           = port::keep_variable;
constexpr port                  keep_reset              = port::keep_reset;
constexpr low_power_mode        keep_clock              = low_power_mode::keep_clock;
constexpr low_power_mode        close_clock_when_sleep  = low_power_mode::close_clock_when_sleep;
constexpr clear_t               clear {};
constexpr bool                  enable          = true;
constexpr bool                  disable         = false;
constexpr div2_t                div2 {};
constexpr div3_t                div3 {};
constexpr div4_t                div4 {};
constexpr div5_t                div5 {};
constexpr div6_t                div6 {};
constexpr div7_t                div7 {};
constexpr div8_t                div8 {};
constexpr div9_15_t             div9 {9}, div10 {10}, div11 {11}, div12 {12}, div13 {13}, div14 {14}, div15 {15};
constexpr div16_31_t            div16 {16}, div17 {17}, div18 {18}, div19 {19}, div20 {20}, div21{21}, div22 {22}, div23 {23}, div24 {24}, div25 {25}, div26 {26}, div27 {27}, div28 {28}, div29 {29}, div30 {30}, div31 {31};
constexpr source_from_sclk      from_sclk {};
constexpr source_from_sclk_div  from_sclk_div2 {8}, from_sclk_div4 {9}, from_sclk_div8 {10}, from_sclk_div16 {11}, from_sclk_div64 {12}, from_sclk_div128 {13}, from_sclk_div256 {14}, from_sclk_div512 {15};
constexpr source_from_ahb       from_ahb {0}, from_ahb_div2 {4}, from_ahb_div4 {5}, from_ahb_div8 {6}, from_ahb_div16{7};
constexpr source_from_hsi       from_hsi {};
constexpr source_from_hse       from_hse {};
constexpr source_from_pll       from_pll {};
constexpr source_from_plli2s    from_plli2s {};
constexpr source_for_i2s        from_pin_i2s_ckin       = source_for_i2s::from_pin_i2s_ckin;
constexpr source_for_rtc        from_lsi                = source_for_rtc::from_lsi;
constexpr drive_capability      low                     = drive_capability::low;
constexpr drive_capability      middle_high             = drive_capability::middle_high;
constexpr drive_capability      middle_low              = drive_capability::middle_low;
constexpr drive_capability      high                    = drive_capability::high;
constexpr spread_mode           center                  = spread_mode::center;
constexpr spread_mode           down                    = spread_mode::down;









        // DAC
        constexpr source_for_channel_trigger from_tim6          = source_for_channel_trigger::from_tim6;
        constexpr source_for_channel_trigger from_tim8          = source_for_channel_trigger::from_tim8;
        constexpr source_for_channel_trigger from_tim7          = source_for_channel_trigger::from_tim7;
        constexpr source_for_channel_trigger from_tim5          = source_for_channel_trigger::from_tim5;
        constexpr source_for_channel_trigger from_tim2          = source_for_channel_trigger::from_tim2;
        constexpr source_for_channel_trigger from_tim4          = source_for_channel_trigger::from_tim4;
        constexpr source_for_channel_trigger from_exti_line9    = source_for_channel_trigger::from_exti_line9;
        constexpr source_for_channel_trigger from_software      = source_for_channel_trigger::from_software;
        constexpr ware_type                  none_ware          = ware_type::none_ware;
        constexpr ware_type                  noise_ware         = ware_type::noise_ware;
        constexpr ware_type                  triangle_ware      = ware_type::triangle_ware;
        constexpr triangle_ware_amplitude    equals_to_1        = triangle_ware_amplitude::equals_to_1;
        constexpr triangle_ware_amplitude    equals_to_3        = triangle_ware_amplitude::equals_to_3;
        constexpr triangle_ware_amplitude    equals_to_7        = triangle_ware_amplitude::equals_to_7;
        constexpr triangle_ware_amplitude    equals_to_15       = triangle_ware_amplitude::equals_to_15;
        constexpr triangle_ware_amplitude    equals_to_31       = triangle_ware_amplitude::equals_to_31;
        constexpr triangle_ware_amplitude    equals_to_63       = triangle_ware_amplitude::equals_to_63;
        constexpr triangle_ware_amplitude    equals_to_127      = triangle_ware_amplitude::equals_to_127;
        constexpr triangle_ware_amplitude    equals_to_255      = triangle_ware_amplitude::equals_to_255;
        constexpr triangle_ware_amplitude    equals_to_511      = triangle_ware_amplitude::equals_to_511;
        constexpr triangle_ware_amplitude    equals_to_1023     = triangle_ware_amplitude::equals_to_1023;
        constexpr triangle_ware_amplitude    equals_to_2047     = triangle_ware_amplitude::equals_to_2047;
        constexpr triangle_ware_amplitude    equals_to_4095     = triangle_ware_amplitude::equals_to_4095;
        constexpr noise_ware_range           less_than_2        = noise_ware_range::less_than_2;
        constexpr noise_ware_range           less_than_4        = noise_ware_range::less_than_4;
        constexpr noise_ware_range           less_than_8        = noise_ware_range::less_than_8;
        constexpr noise_ware_range           less_than_16       = noise_ware_range::less_than_16;
        constexpr noise_ware_range           less_than_32       = noise_ware_range::less_than_32;
        constexpr noise_ware_range           less_than_64       = noise_ware_range::less_than_64;
        constexpr noise_ware_range           less_than_128      = noise_ware_range::less_than_128;
        constexpr noise_ware_range           less_than_256      = noise_ware_range::less_than_256;
        constexpr noise_ware_range           less_than_512      = noise_ware_range::less_than_512;
        constexpr noise_ware_range           less_than_1024     = noise_ware_range::less_than_1024;
        constexpr noise_ware_range           less_than_2048     = noise_ware_range::less_than_2048;
        constexpr noise_ware_range           less_than_4096     = noise_ware_range::less_than_4096;
