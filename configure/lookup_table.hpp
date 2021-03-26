#ifndef xpack_configure_lookup_table
#define xpack_configure_lookup_table

/* mixc::math =======================================================
 * 
 * =================================================================*/
#define xuse_math_tan_lut8x16       1
#define xuse_math_tan_lut4x256      0

#define xuse_math_atan_lut7x16      1
#define xuse_math_atan_lut4x256     0

#define xuse_math_ln_lut6x16        1
#define xuse_math_ln_lut3x256       0

#define xuse_math_expe_lut6x16      1
#define xuse_math_expe_lut3x256     0

#define xuse_math_sqrt_lut6x16      1
#define xuse_math_sqrt_lut3x256     0

#if xuse_math_tan_lut8x16 + xuse_math_tan_lut4x256 > 1
    #error "you can select at least one lookup table policy for math::tan"
#endif

#if xuse_math_atan_lut7x16 + xuse_math_atan_lut4x256 > 1
    #error "you can select at least one lookup table policy for math::atan"
#endif

#if xuse_math_ln_lut6x16 + xuse_math_ln_lut3x256 > 1
    #error "you can select at least one lookup table policy for math::ln"
#endif

#if xuse_math_expe_lut6x16 + xuse_math_expe_lut3x256 > 1
    #error "you can select at least one lookup table policy for math::expe"
#endif

#if xuse_math_sqrt_lut6x16 + xuse_math_sqrt_lut3x256 > 1
    #error "you can select at least one lookup table policy for math::sqrt"
#endif

#endif
