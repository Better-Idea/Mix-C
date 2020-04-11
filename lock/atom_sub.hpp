#ifndef xpack_lock_atom_sub
#define xpack_lock_atom_sub
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lock_atom_sub
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lock_atom_sub{
        template<class a>
        inline a atom_sub(a * left, a right){
            return __atomic_sub_fetch(left, right, 5);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::lock_atom_sub::atom_sub;
}
