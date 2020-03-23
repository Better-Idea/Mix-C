#ifndef xpack_lock_atom_fetch_and
#define xpack_lock_atom_fetch_and
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lock_atom_fetch_and
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lock_atom_fetch_and{
        template<class a>
        inline a atom_fetch_and(a * left, a right){
            return __atomic_fetch_and(left, right, 5);
        }
    }

#endif

namespace xuser::inc{
    using namespace mixc::lock_atom_fetch_and;
}
