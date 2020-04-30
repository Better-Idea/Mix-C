#ifndef xpack_instruction_index_of_first_set
#define xpack_instruction_index_of_first_set
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::instruction_index_of_first_set
        #include"define/base_type.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::instruction_index_of_first_set{
        template<class type>
        inline uxx index_of_first_set(type v){
            return __builtin_ffs(v) - 1;
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::instruction_index_of_first_set::index_of_first_set;
}
