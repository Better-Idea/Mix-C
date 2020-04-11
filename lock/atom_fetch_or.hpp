#ifndef xpack_lock_atom_fetch_or
#define xpack_lock_atom_fetch_or
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lock_atom_fetch_or
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lock_atom_fetch_or{
        template<class a>
        inline a atom_fetch_or(a * left, a right){
            return __atomic_fetch_or(left, right, 5);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::lock_atom_fetch_or::atom_fetch_or;
}
