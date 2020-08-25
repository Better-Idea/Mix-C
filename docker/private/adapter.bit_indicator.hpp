#ifndef xpack_docker_adapter_bit_indicator
#define xpack_docker_adapter_bit_indicator
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_adapter_bit_indicator
#include"define/base_type.hpp"
#include"instruction/index_of_first_set.hpp"
#include"instruction/index_of_last_set.hpp"
#include"interface/ranger.hpp"
#include"macro/xstruct.hpp"
#include"memop/zeros.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_adapter_bit_indicator::origin{
    #if xis_os64
        static constexpr uxx step_exp = 6;
        static constexpr uxx bwidth   = 64;
        static constexpr uxx mask     = 0x3f;
    #else
        static constexpr uxx step_exp = 5;
        static constexpr uxx bwidth   = 32;
        static constexpr uxx mask     = 0x1f;
    #endif

    template<class callback>
    inline void bmp_set(uxx index, uxx * bmp, const uxx * level_lut, uxx height, callback invoke){
        uxx i = 0;
        uxx j = 0;

        while(true){
            if (j = index >> step_exp; invoke(bmp[j], uxx(1) << (index & mask))){
                break;
            }
            if (i + 1 == height){
                break;
            }

            bmp    += level_lut[i++];
            index   = j;
        }
    }

    template<class callback>
    inline uxx bmp_find(uxx * bmp, uxx size, const uxx * level_lut, uxx height, callback invoke){
        uxx  offset = 0;
        uxx  index  = 0;
        uxxp p      = bmp + size;

        if (p[-1] == 0){
            return not_exist;
        }

        for(uxx i = height; i-- > 0;){
            p      -= level_lut[i];
            offset  = invoke(p[index]);
            index   = index << step_exp | offset;
        }
        return index;
    }

    template<class callback>
    inline uxx bmp_find(uxx * bmp, const uxx * level_lut, uxx height, callback invoke, uxx boundary){
        auto is_find_last = invoke == & inc::index_of_last_set<uxx>;
        uxx  w      = boundary >> step_exp;

        if (not is_find_last and w >= level_lut[0]){
            return not_exist;
        }

        uxx  i      = 0;
        uxx  msk    = is_find_last ? uxx(-1) >> (bwidth - 1 - (boundary & mask)) : uxx(-1) << (boundary & mask);
        uxx  r      = invoke(bmp[w] & msk);

        if (r != not_exist){
            return w << step_exp | r;
        }

        while(i < height - 1){
            // 从下向上
            bmp    += level_lut[i];
            msk     = is_find_last ? uxx(-1) >> (bwidth - (w & mask)) : uxx(-1) << ((w & mask) + 1);
            w     >>= step_exp;
            r       = invoke(bmp[w] & msk);

            if (r == not_exist){
                i  += 1;
                continue;
            }

            // 从上向下
            do{
                bmp -= level_lut[i];
                w    = w << step_exp | r;
                r    = invoke(bmp[w]);
            } while(i-- != 0);

            return w << step_exp | r;
        }
        return not_exist;
    }
    
    template<class base_t>
    /*
    needed:
    - base_t::bmp()
    - base_t::size()
    - base_t::level_lut()
    - base_t::height()
    */
    xstruct(
        xname(adapter_bit_indicator_t),
        xpubb(base_t)
    )
        using base_t::base_t;

        void clear(){
            inc::zeros(base_t::bmp(), base_t::size() * sizeof(uxx));
        }

        void set(uxx index){
            bmp_set(index, base_t::bmp(), base_t::level_lut(), base_t::height(), [](uxx & p, uxx val){
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
            bmp_set(index, base_t::bmp(), base_t::level_lut(), base_t::height(), [](uxx & p, uxx val){
                p &= ~val;
                return p != 0;
            });
        }

        void reset(inc::ranger<uxx> const & index_list){
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
            return bmp_find(
                base_t::bmp(),
                base_t::level_lut(),
                base_t::height(),
                & inc::index_of_first_set<uxx>, start_boundary
            );
        }

        uxx index_of_last_set(uxx end_boundary){
            return bmp_find(
                base_t::bmp(),
                base_t::level_lut(),
                base_t::height(),
                & inc::index_of_last_set<uxx>, end_boundary
            );
        }

        uxx index_of_first_set(){
            return bmp_find(
                base_t::bmp(),
                base_t::size(),
                base_t::level_lut(),
                base_t::height(),
                & inc::index_of_first_set<uxx>
            );
        }

        uxx index_of_last_set(){
            return bmp_find(
                base_t::bmp(),
                base_t::size(),
                base_t::level_lut(),
                base_t::height(),
                & inc::index_of_last_set<uxx>
            );
        }

        bool get(uxx index){
            auto i  = index >> step_exp;
            auto m  = uxx(1) << (index & mask);
            return (base_t::bmp()[i] & m) != 0;
        }
    $

    template<class final, class base>
    struct adapter_bit_indicator : adapter_bit_indicator_t<base>{
        using the_t = adapter_bit_indicator_t<base>;
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

namespace xuser::inc{
    using namespace ::mixc::docker_adapter_bit_indicator::origin;
}
