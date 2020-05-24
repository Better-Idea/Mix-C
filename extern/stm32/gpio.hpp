#ifndef xpack_extern_stm32_gpio
#define xpack_extern_stm32_gpio
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::extern_stm32_gpio
        #include"extern/stm32/afio.hpp"
        #include"extern/stm32/define.hpp"
        #include"meta/is_based_on.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::extern_stm32_gpio::origin{
        struct _gpio{
        private:
            static constexpr u32 port_map[] = {
                0x40020000, // gpioa
                0x40020400, // gpiob
                0x40020800, // gpioc
                0x40020c00, // gpiod
                0x40021000, // gpioe
                0x40021400, // gpiof
                0x40021800, // gpiog
                0x40021c00, // gpioh
                0x40022000, // gpioi
                0x40022400, // gpioj
                0x40022800, // gpiok
            };

            union config_reflect{
                template<class type>
                config_reflect(type pin){
                    if constexpr (inc::is_based_on<afio_base, type>){
                        field = pin.io;
                    }
                    else{
                        field = pin;
                    }
                }

            private:
                u32     field;
            public:
                struct{
                    u08 af;
                    u08 pin;
                    u08 port;
                };
            };

            struct mode_t{
                u32 up_down : 2;
                u32 speed   : 2;
                u32 od      : 1;
                u32 mode    : 2;

                mode_t(io_mode value){
                    *u32p(this) = u32(value);
                }
            };

            enum {
                xfield(iomode, 32),
                xfield(otype , 32),
                xfield(speed , 32),
                xfield(pupd  , 32),
                xfield(idr   , 32),
                xfield(odr   , 32),
                xfield(bsr   , 32),
                xfield(lckr  , 32),
                xfield(afrl  , 64),
            };

            using the_t = _gpio;
        public:
            constexpr _gpio(){}

            template<class ... args>
            auto & config(io_mode mode, args ... ports) const {
                config_reflect  items[] = { ports... };
                mode_t          props = mode;
                u32             base;
                u32             start;
                u32             end;

                for(config_reflect item : items){
                    base  = port_map[item.port];
                    start = otype + item.pin;
                    end   = start;      // 1bit
                    mcu_bop_direct(base, start, end, props.od);

                    start = speed + item.pin * 2;
                    end   = start + 1;  // 2bit
                    mcu_bop_direct(base, start, end, props.speed);

                    start = pupd + item.pin * 2;
                    end   = start + 1;  // 2bit
                    mcu_bop_direct(base, start, end, props.up_down);

                    start = afrl + item.pin * 4;
                    end   = start + 3;  // 4bit
                    mcu_bop_direct(base, start, end, item.af);

                    start = iomode + item.pin * 2;
                    end   = start + 1;  // 2bit
                    mcu_bop_direct(base, start, end, props.mode);
                }
                return the;
            }

            template<class type>
            auto get(type pin) const {
                config_reflect item  = pin;
                u32            base  = port_map[item.port];
                u32            start = idr + item.pin;
                u32            end   = start;
                return mcu_bop(base, start, end);
            }

            template<class type>
            auto & get(type pin, bool * value) const {
                value[0] = get(pin);
                return the;
            }

            template<class type>
            auto & set(type pin, bool value) const {
                asm("":::"memory");
                config_reflect item  = pin;
                u32            base  = port_map[item.port];
                u32            start = odr + item.pin;
                u32            end   = start;
                mcu_bop_direct(base, start, end, value);
                asm("":::"memory");
                return the;
            }

            template<class type>
            auto & set(type pin) const {
                return set(pin, 1);
            }

            template<class type>
            auto & reset(type pin) const {
                return set(pin, 0);
            }

            auto & flush() const {
                asm("":::"memory");
                return the;
            }
        };

        constexpr _gpio gpio{};
    }
#endif

namespace xuser::inc{
    using namespace mixc::extern_stm32_gpio::origin;
}
