#ifndef xpack_docker_private_bit_indicator
#define xpack_docker_private_bit_indicator
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::docker_bit_indicator
    #include"docker/private/bit_indicator_adapter.hpp"
    #include"macro/xstruct.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_bit_indicator{
        template<uxx bits>
        xstruct(
            xtmpl(bit_indicator_t, bits)
        )
        public:
            constexpr uxx length(){
                return bits;
            }
        private:
            static constexpr uxx lv0 = bits / inc::bwidth + (bits % inc::bwidth != 0);
            static constexpr uxx lv1 = lv0 <= 1 ? 0 : lv0 / inc::bwidth + (lv0 % inc::bwidth != 0);
            static constexpr uxx lv2 = lv1 <= 1 ? 0 : lv1 / inc::bwidth + (lv1 % inc::bwidth != 0);
            static constexpr uxx lv3 = lv2 <= 1 ? 0 : lv2 / inc::bwidth + (lv2 % inc::bwidth != 0);
            static constexpr uxx lv4 = lv3 <= 1 ? 0 : lv3 / inc::bwidth + (lv3 % inc::bwidth != 0);
            static constexpr uxx lv5 = lv4 <= 1 ? 0 : lv4 / inc::bwidth + (lv4 % inc::bwidth != 0);
            static constexpr uxx lv6 = lv5 <= 1 ? 0 : lv5 / inc::bwidth + (lv5 % inc::bwidth != 0);
            static constexpr uxx lv7 = lv6 <= 1 ? 0 : lv6 / inc::bwidth + (lv6 % inc::bwidth != 0);
            static constexpr uxx lv8 = lv7 <= 1 ? 0 : lv7 / inc::bwidth + (lv7 % inc::bwidth != 0);
            static constexpr uxx lv9 = lv8 <= 1 ? 0 : lv8 / inc::bwidth + (lv8 % inc::bwidth != 0);
            static constexpr uxx lut[] = { lv0,lv1,lv2,lv3,lv4,lv5,lv6,lv7,lv8,lv9 };
        protected:
            static const uxx * level_lut(){
                return lut;
            }

            static constexpr uxx size(){
                return lv0 + lv1 + lv2 + lv3 + lv4 + lv5 + lv6 + lv7 + lv8 + lv9;
            }

            static constexpr uxx height(){
                return 
                    lv9 ? 10 : 
                    lv8 ? 9  : 
                    lv7 ? 8  : 
                    lv6 ? 7  : 
                    lv5 ? 6  : 
                    lv4 ? 5  : 
                    lv3 ? 4  : 
                    lv2 ? 3  : 
                    lv1 ? 2  : 1;
            }

            uxx * bmp(){
                return data;
            }

            uxx data[size()] = {0};
        $
        
        template<class final, uxx bits>
        using bit_indicator = inc::bit_indicator<final, bit_indicator_t<bits>>;
    }

#endif

#undef  xusing_docker_bit_indicator
#define xusing_docker_bit_indicator     ::mixc::docker_bit_indicator
