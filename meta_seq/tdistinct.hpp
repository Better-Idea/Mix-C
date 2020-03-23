
#ifndef xpack_meta_seq_tdistinct
#define xpack_meta_seq_tdistinct
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tdistinct
        #include"define/base_type.hpp"
        #include"meta_seq/tlist.hpp"
        #include"meta_seq/tin.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tdistinct{
        using namespace inc;

        template<class list>
        struct tdistinct{
        private:
            template<class first, class ... args, class ... result_args>
            static auto invoke(tlist<first, args...>, tlist<result_args...>){
                using old_list = tlist<result_args...>;
                using new_list = tlist<result_args..., first>;
                if constexpr(tin<tlist<result_args...>, first>){
                    return invoke(
                        tlist<args...>(),
                        old_list()
                    );
                }
                else{
                    return invoke(
                        tlist<args...>(),
                        new_list()
                    );
                }
            }

            template<class result>
            static auto invoke(tlist<>, result r){
                return r;
            }
        public:
            using new_list = decltype(
                invoke(list(), tlist<>())
            );
        };
    }

#endif

namespace xuser::inc{
    template<class tlist>
    using tdistinct = 
        typename mixc::meta_seq_tdistinct::tdistinct<tlist>;
}
