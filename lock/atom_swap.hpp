#ifndef xpack_lock_atom_swap
#define xpack_lock_atom_swap
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lock_atom_swap
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lock_atom_swap{
        template<class a>
        inline a atom_swap(a * left, a right){
            // function equals to 
            // r = left[0]
            // left[0] = right
            a r;
            __atomic_exchange(left, & right, & r, 5);
            return r;
        }
    }

#endif

namespace xuser::inc{
    using namespace mixc::lock_atom_swap;
}
