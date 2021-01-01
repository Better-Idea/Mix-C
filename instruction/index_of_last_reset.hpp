#ifndef xpack_instruction_index_of_last_reset
#define xpack_instruction_index_of_last_reset
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::instruction_index_of_last_reset::inc
#include"instruction/index_of_last_set.hpp
#pragma pop_macro("xuser")

namespace mixc::instruction_index_of_last_reset{
    template<class type>
    inline uxx index_of_last_reset(type v){
        // ~v 可能会由 unsigned char 之类的类型变为 int 类型，所以需要显式指定模板参数
        return inc::index_of_last_set<type>(~v);
    }
}

#endif

xexport(mixc::instruction_index_of_last_reset::index_of_last_reset)