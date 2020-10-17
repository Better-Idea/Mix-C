#ifndef xpack_docker_adapter_array_access
#define xpack_docker_adapter_array_access
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_adapter_array_access
#include"interface/seqptr.hpp"
#include"interface/iterator.hpp"
#include"meta/is_integer.hpp"
#include"memop/swap.hpp"
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_adapter_array_access {
    template<class base_t, class item_t>
    struct adapter_array_access : base_t {
        using the_t             = base_t;
        using base_t::base_t;
        using base_t::operator[];
        using base_t::length;

        xseqptr(item_t);
    private:
        template<inc::is_integer number_t>
        auto & random_access(number_t const & index) const {
            if constexpr (number_t(-1) > 0) {
                return base_t::operator[](uxx(index));
            }
            else {
                return base_t::operator[](index >= 0 ? uxx(index) : uxx(length() + index));
            }
        }

        template<auto mode, class invoke_t>
        void foreach_template(invoke_t invoke, inc::iinterval itv = co{0, -1}) const {
            itv.normalize(the.length());

            uxx    l     = itv.left();
            uxx    r     = itv.right();
            uxx    step  = l <= r ? uxx(1) : uxx(-1);
            uxx    index = 0;
            loop_t state = loop_t::go_on;

            for(;; l += step){
                xitr_switch(mode, index, state, invoke, the[l]);
                
                if (l == r or state == loop_t::finish){
                    break;
                }
            }
        }
    public:
        template<inc::is_integer number_t>
        item_t & operator[] (number_t const & index) {
            return random_access(index);
        }

        template<inc::is_integer number_t>
        item_t const & operator[] (number_t const & index) const {
            return random_access(index);
        }
        
        xitr_foreach (item_t &)
        xitr_foreachx(item_t &)
        xitr_foreach_const (item_t const &)
        xitr_foreachx_const(item_t const &)
    };
}

#endif

xexport(mixc::docker_adapter_array_access::adapter_array_access)
