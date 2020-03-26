
#ifndef xpack_meta_seq_vget
#define xpack_meta_seq_vget
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_vget
        #include"define/base_type.hpp"
        #include"meta_seq/vlist.hpp"
        #include"meta_seq/vnull.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_vget{
        using namespace inc;

        template<class list, ixx index> struct vget{
        private:
            template<ixx i>
            static constexpr auto invoke(vlist<>){
                return vnull;
            }

            template<ixx i, auto first, auto ... values>
            static constexpr auto invoke(vlist<first, values...>){
                if constexpr (i == 0 or -i == 1 + sizeof...(values)){
                    return first;
                }
                else{
                    return invoke<
                        (i < 0 ? i : i - 1), values...
                    >(vlist<values...>());
                }
            }

        public:
            static constexpr auto value = invoke<index>(list());
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_vget::vget;
}
