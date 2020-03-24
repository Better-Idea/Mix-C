#ifndef xpack_meta_seq_tfilter
#define xpack_meta_seq_tfilter
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tfilter
        #include"define/base_type.hpp"
        #include"meta_seq/tlist.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tfilter{
        using namespace inc;

        template<class list, template <class> class selector>
        struct tfilter{
        private:
            template<class ... result_args>
            static auto invoke(tlist<>, tlist<result_args...> r){
                return r;
            }

            template<class first, class ... args, class ... result_args>
            static auto invoke(tlist<first, args...>, tlist<result_args...>){
                using item = typename selector<first>::item;
                return invoke(
                    tlist<args...>(), 
                    tlist<result_args..., item>()
                );
            }
        public:
            using new_list = decltype(
                invoke(list(), tlist<>())
            );
        };
    }

#endif

namespace xuser::inc{
    using mixc::meta_seq_tfilter::tfilter;
}
