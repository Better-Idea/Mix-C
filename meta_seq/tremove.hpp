#ifndef xpack_meta_seq_tremove
#define xpack_meta_seq_tremove
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tremove
        #include"define/base_type.hpp"
        #include"meta/is_same.hpp"
        #include"meta_seq/tlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tremove{
        using namespace inc;

        template<class list, class item>
        struct tremove{
        private:
            template<class first, class ... args, class ... result_args>
            static auto invoke(tlist<first, args...>, tlist<result_args...>){
                if constexpr(is_same<first, item>){
                    return invoke(tlist<args...>(), tlist<result_args...>());
                }
                else{
                    return invoke(tlist<args...>(), tlist<result_args..., first>());
                }
            }

            template<class result>
            static auto invoke(tlist<>, result r){
                return r;
            }
        public:
            using new_list = decltype(invoke(list(), tlist<>()));
        };
    }

#endif

namespace xuser::inc{
    using mixc::meta_seq_tremove::tremove;
}
