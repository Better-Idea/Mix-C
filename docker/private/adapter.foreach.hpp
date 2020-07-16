#ifndef xpack_docker_adapter_foreach
#define xpack_docker_adapter_foreach
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_adapter_foreach
#include"interface/seqptr.hpp"
#include"interface/iterator.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_adapter_foreach {
    template<class final, class base_t, class item_t>
    struct adapter_foreach : base_t {
        using the_t     = base_t;
        using iteratorx = inc::iteratorx<item_t &> const &;
        using iterator  = inc::iterator <item_t &> const &;
        using base_t::base_t;
        using base_t::operator[];
        using base_t::length;

        xseqptr(item_t);

        final & foreach(inc::iinterval i, iteratorx itr){
            auto r = range(i);

            for(uxx i = 0; i < r.length(); i++){
                if (itr(i, the[i]) == loop_t::finish){
                    break;
                }
            }
            return thex;
        }

        final & foreach(inc::iinterval i, iterator itr){
            return foreach(i, [&](uxx index, item_t & item){
                itr(index, item);
                return loop_t::go_on;
            });
        }

        final & foreach(iteratorx itr){
            return foreach(co{0, -1}, itr);
        }

        final & foreach(iterator itr){
            return foreach(co{0, -1}, itr);
        }
    };
}

#endif

namespace xuser::inc {
	using ::mixc::docker_adapter_foreach::adapter_foreach;
}
