#ifndef xpack_lock_atom_fetch_add
#define xpack_lock_atom_fetch_add
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lock_atom_fetch_add
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lock_atom_fetch_add{
        template<class a>
        inline a atom_fetch_add(a * left, a right){
            return __atomic_fetch_add(left, right, 5);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::lock_atom_fetch_add::atom_fetch_add;
}
