#ifndef xpack_dumb_partial
#define xpack_dumb_partial
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::dumb_partial
        #include"define/base_type.hpp"
        #include"meta/is_same.hpp"
        #include"meta/is_based_on.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::dumb_partial{
        template<class base>
        struct partial{
            template<class target>  
            operator target & () const {
                static_assert(
                    inc::is_based_on<base, target> or 
                    inc::is_based_on<partial<base>, target> or
                    inc::is_same<base, target>
                );
                return *(target *)this;
            }
        };
    }

#endif

namespace xuser::inc{
    using namespace mixc::dumb_partial;
}
