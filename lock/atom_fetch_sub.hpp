#ifndef xpack_lock_atom_fetch_sub
#define xpack_lock_atom_fetch_sub
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lock_atom_fetch_sub
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lock_atom_fetch_sub{
        template<class a>
        inline a atom_fetch_sub(a * left, a right){
            return __atomic_fetch_sub(left, right, 5);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::lock_atom_fetch_sub::atom_fetch_sub;
}
