#ifndef xpack_docker_adapter_pushpop
#define xpack_docker_adapter_pushpop
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::docker_adapter_pushpop
#include"define/base_type.hpp"
#include"interface/ranger.hpp"
#pragma pop_macro("xuser")

namespace mixc::docker_adapter_pushpop {
    template<class final, class base_t, class item_t>
    struct adapter_pushpop : base_t {
        using the_t = base_t;
        using the_t::the_t;
        using the_t::pop;

        final & clear() {
            the.clear();
            return thex;
        }

        final & push(item_t const & value) {
            the.push(value);
            return thex;
        }

        final & push(inc::ranger<item_t> values) {
            for(uxx i = 0; i < values.length(); i++) {
                the.push(values[i]);
            }
            return thex;
        }

        final & pop(item_t * value) {
            value[0] = the.pop();
            return thex;
        }

        final & pop(inc::ranger<item_t *> values) {
            for(uxx i = 0; i < values.length(); i++){
                values[i][0] = the.pop();
            }
            return thex;
        }
    };
}

#endif

namespace xuser::inc {
	using ::mixc::docker_adapter_pushpop::adapter_pushpop;
}
