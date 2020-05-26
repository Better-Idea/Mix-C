#ifndef xpack_meta_seq_tpop_by
#define xpack_meta_seq_tpop_by
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tpop_by
        #include"define/base_type.hpp"
        #include"meta/is_same.hpp"
        #include"meta_seq/tlist.hpp"
        #include"meta_seq/tselector_key.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tpop_by{
        using namespace inc;

        template<class list, class key, template<class> class selector>
        struct tpop_by{
        private:
            template<class first, class ... args, class ... vals, class ... rest>
            static auto invoke(tlist<first, args...>, tlist<tlist<vals...> , tlist<rest...>>){
                tlist<args...> src;
                using current_key = typename selector<first>::item;

                if constexpr(is_same<current_key, key>){
                    return invoke(src, tlist<tlist<vals..., first> , tlist<rest...>> ());
                }
                else{
                    return invoke(src, tlist<tlist<vals...> , tlist<rest..., first>> ());
                }
            }

            template<class ... vals, class ... rest>
            static auto invoke(tlist<>, tlist<tlist<vals...> , tlist<rest...>> r){
                return r;
            }

            template<class ... vals, class ... rest>
            static auto val(tlist<tlist<vals...>, tlist<rest...>>){
                return tlist<vals...>();
            }

            template<class ... vals, class ... rest>
            static auto res(tlist<tlist<vals...> , tlist<rest...>>){
                return tlist<rest...>();
            }

            using pair = decltype(
                invoke(list(), tlist<tlist<>, tlist<>>())
            );
        public:
            using item_list = decltype(
                val(pair())
            );
            using new_list = decltype(
                res(pair())
            );
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_tpop_by::tpop_by;
}
