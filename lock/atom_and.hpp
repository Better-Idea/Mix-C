#ifndef xpack_lock_atom_and
#define xpack_lock_atom_and
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lock_atom_and
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lock_atom_and{
        template<class a>
        inline a atom_and(a * left, a right){
            return __atomic_and_fetch(left, right, 5);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::lock_atom_and::atom_and;
}
