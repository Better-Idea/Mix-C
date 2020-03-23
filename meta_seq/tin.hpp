#ifndef xpack_meta_seq_tin
#define xpack_meta_seq_tin
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tin
        #include"define/base_type.hpp"
        #include"meta/is_same.hpp"
        #include"meta_seq/tlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tin{
        using namespace inc;

        namespace inner{
            template<class item>
            inline constexpr bool meta(tlist<>){
                return false;
            }

            template<class item, class first, class ... args>
            inline constexpr bool meta(tlist<first, args...>){
                if constexpr (is_same<first, item>){
                    return true;
                }
                else{
                    return meta<item>(tlist<args...>());
                }
            }
        }

        
    }

#endif

namespace xuser::inc{
    template<class tlist, class item>
    constexpr bool tin = mixc::meta_seq_tin::inner::meta<item>(tlist());
}
