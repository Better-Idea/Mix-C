#ifndef xpack_docker_bit_indicator
#define xpack_docker_bit_indicator
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::docker_bit_indicator
        #include"define/base_type.hpp"
        #include"instruction/index_of_first_set.hpp"
        #include"instruction/index_of_last_set.hpp"
        #include"interface/ranger.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::docker_bit_indicator{
        template<uxx bits>
        struct bit_indicator_t{
            void set(uxx index){
                set(index, [](bits_t & p, bits_t val){
                    p |= val;
                    return false;
                });
            }

            void set(inc::ranger<uxx> index_list){
                for(uxx i = 0; i < index_list.length(); i++){
                    set(index_list[i]);
                }
            }

            void reset(uxx index){
                set(index, [](bits_t & p, bits_t val){
                    p &= ~val;
                    return p != 0;
                });
            }

            void reset(inc::ranger<uxx> index_list){
                for(uxx i = 0; i < index_list.length(); i++){
                    reset(index_list[i]);
                }
            }

            uxx pop_first(){
                if (uxx index = index_of_first_set(); index != not_exist){
                    reset(index);
                    return index;
                }
                return not_exist;
            }

            uxx pop_last(){
                if (uxx index = index_of_last_set(); index != not_exist){
                    reset(index);
                    return index;
                }
                return not_exist;
            }

            uxx index_of_first_set(uxx start_boundary){
                return find(& inc::index_of_first_set<bits_t>, start_boundary);
            }

            uxx index_of_last_set(uxx end_boundary){
                return find(& inc::index_of_last_set<bits_t>, end_boundary);
            }

            uxx index_of_first_set(){
                return find(& inc::index_of_first_set<bits_t>);
            }

            uxx index_of_last_set(){
                return find(& inc::index_of_last_set<bits_t>);
            }

            bool get(uxx index){
                auto i  = index >> step_exp;
                auto m  = bits_t(1) << (index & mask);
                return (data[i] & m) != 0;
            }

            constexpr uxx length() const {
                return bits;
            }
        private:
            template<class callback>
            uxx find(callback invoke){
                uxx  lv     = 0;
                uxx  offset = 0;
                uxx  index  = 0;
                uxxp p      = data + size;

                if (p[-1] == 0){
                    return not_exist;
                }

                for(uxx i = height; i-- > 0;){
                    lv      = lut[i];
                    p      -= lv;
                    offset  = invoke(p[index]);
                    index   = index << step_exp | offset;
                }
                return index;
            }

            template<class callback>
            uxx find(callback invoke, uxx boundary){
                auto is_find_last = invoke == & inc::index_of_last_set<bits_t>;
                auto idc    = data;
                uxx  i      = 0;
                uxx  msk    = is_find_last ? uxx(-1) >> (width - 1 - (boundary & mask)) : uxx(-1) << (boundary & mask);
                uxx  w      = boundary >> step_exp;
                uxx  r      = invoke(idc[w] & msk);

                if (r != not_exist){
                    return w << step_exp | r;
                }

                while(i < height - 1){
                    // 从下向上
                    idc    += lut[i];
                    msk     = is_find_last ? uxx(-1) >> (width - (w & mask)) : uxx(-1) << ((w & mask) + 1);
                    w     >>= step_exp;
                    r       = invoke(idc[w] & msk);

                    if (r == not_exist){
                        i  += 1;
                        continue;
                    }

                    // 从上向下
                    do{
                        idc -= lut[i];
                        w    = w << step_exp | r;
                        r    = invoke(idc[w]);
                    } while(i-- != 0);

                    return w << step_exp | r;
                }
                return not_exist;
            }

            template<class callback>
            void set(uxx index, callback invoke){
                uxx  i      = 0;
                uxx  j      = 0;
                uxx  lv     = 0;
                uxxp p      = data;

                while(true){
                    j       = index >> step_exp;

                    if (invoke(p[j], bits_t(1) << (index & mask))){
                        break;
                    }

                    if (i + 1 == height){
                        break;
                    }

                    lv      = lut[i++];
                    p      += lv;
                    index >>= step_exp;
                }
            }

            #if xis_os64
                using bits_t                  = u64;
                static constexpr uxx step_exp = 6;
                static constexpr uxx width    = 64;
                static constexpr uxx mask     = 0x3f;
            #else
                using bits_t                  = u32;
                static constexpr uxx step_exp = 5;
                static constexpr uxx width    = 32;
                static constexpr uxx mask     = 0x1f;
            #endif

            static constexpr uxx lv0    = bits / width + (bits % width != 0);
            static constexpr uxx lv1    = lv0 <= 1 ? 0 : lv0  / width + (lv0  % width != 0);
            static constexpr uxx lv2    = lv1 <= 1 ? 0 : lv1  / width + (lv1  % width != 0);
            static constexpr uxx lv3    = lv2 <= 1 ? 0 : lv2  / width + (lv2  % width != 0);
            static constexpr uxx lv4    = lv3 <= 1 ? 0 : lv3  / width + (lv3  % width != 0);
            static constexpr uxx lv5    = lv4 <= 1 ? 0 : lv4  / width + (lv4  % width != 0);
            static constexpr uxx lv6    = lv5 <= 1 ? 0 : lv5  / width + (lv5  % width != 0);
            static constexpr uxx lv7    = lv6 <= 1 ? 0 : lv6  / width + (lv6  % width != 0);
            static constexpr uxx lv8    = lv7 <= 1 ? 0 : lv7  / width + (lv7  % width != 0);
            static constexpr uxx lv9    = lv8 <= 1 ? 0 : lv8  / width + (lv8  % width != 0);

            static constexpr uxx lut[]  = { lv0,lv1,lv2,lv3,lv4,lv5,lv6,lv7,lv8,lv9 };
            static constexpr uxx size   = lv0 + lv1 + lv2 + lv3 + lv4 + lv5 + lv6 + lv7 + lv8 + lv9;
            static constexpr uxx height = 
                lv9 ? 10 : 
                lv8 ? 9  : 
                lv7 ? 8  : 
                lv6 ? 7  : 
                lv5 ? 6  : 
                lv4 ? 5  : 
                lv3 ? 4  : 
                lv2 ? 3  : 
                lv1 ? 2  : 1;

            bits_t data[size] = {0};

            xgc_fields(
                xiam(bit_indicator_t<bits>)
            );
        };

        template<class final, uxx bits>
        struct bit_indicator : bit_indicator_t<bits>{
            using the_t = bit_indicator_t<bits>;
            using the_t::the_t;
            using the_t::pop_first;
            using the_t::pop_last;
            using the_t::index_of_first_set;
            using the_t::index_of_last_set;
            using the_t::get;

            final & pop_first(uxx * value){
                value[0] = pop_first();
                return thex;
            }

            final & pop_last(uxx * value){
                value[0] = pop_last();
                return thex;
            }

            final & index_of_first_set(uxx * value){
                value[0] = index_of_first_set();
                return thex;
            }
            
            final & index_of_first_set(uxx start_boundary, uxx * value){
                value[0] = index_of_first_set(start_boundary);
                return thex;
            }
            
            final & index_of_last_set(uxx end_boundary, uxx * value){
                value[0] = index_of_last_set(end_boundary);
                return thex;
            }

            final & index_of_last_set(uxx * value){
                value[0] = index_of_last_set();
                return thex;
            }

            final & get(uxx index, bool * value){
                value[0] = get(index);
                return thex;
            }

            final & set(uxx index){
                the.set(index);
                return thex;
            }

            final & set(inc::ranger<uxx> index_group){
                the.set(index_group);
                return thex;
            }

            final & reset(uxx index){
                the.reset(index);
                return thex;
            }

            final & reset(inc::ranger<uxx> index_group){
                the.reset(index_group);
                return thex;
            }
        };
    }
#endif

#undef  xusing_docker_bit_indicator
#define xusing_docker_bit_indicator     mixc::docker_bit_indicator
