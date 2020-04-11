#ifndef xpack_lock_atom_add
#define xpack_lock_atom_add
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lock_atom_add
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lock_atom_add{
        template<class a>
        inline a atom_add(a * left, a right){
            return __atomic_add_fetch(left, right, 5);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::lock_atom_add::atom_add;
}
