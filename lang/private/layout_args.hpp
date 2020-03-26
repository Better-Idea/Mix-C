#ifndef xpack_lang_private_layout
#define xpack_lang_private_layout
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lang_private_layout
        #include"define/base_type.hpp"
        #include"memory/alloc_callback.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lang_private_layout{
        template<class item, uxx i> 
        inline auto layout_args(item * buf, inc::alloc_callback<item> alloc){
            return alloc;
        }

        template<class item, uxx i>
        inline auto layout_args(item * buf){
            return buf;
        }

        template<class item, uxx i = 0, class ... args> 
        inline auto layout_args(
            item *           buf, 
            item             first, 
            args const & ... list){
            buf[i] = first;
            return layout_args<item, i + 1>(buf, list...);
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::lang_private_layout::layout_args;
}
