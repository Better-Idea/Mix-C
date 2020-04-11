#ifndef xpack_lock_atom_fetch_xor
#define xpack_lock_atom_fetch_xor
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lock_atom_fetch_xor
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lock_atom_fetch_xor{
        template<class a>
        inline a atom_fetch_xor(a * left, a right){
            return __atomic_fetch_xor(left, right, 5);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::lock_atom_fetch_xor::atom_fetch_xor;
}
