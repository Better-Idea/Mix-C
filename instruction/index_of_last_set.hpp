#ifndef xpack_instruction_index_of_last_set
#define xpack_instruction_index_of_last_set
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::instruction_index_of_last_set
        #include"define/base_type.hpp"
        #include"define/platform.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::instruction_index_of_last_set{
        template<class type>
        inline uxx index_of_last_set(type v){
            return v == 0 ? not_exist : uxx(sizeof(v) * 8 - 1 - __builtin_clz(v));
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::instruction_index_of_last_set::index_of_last_set;
}
