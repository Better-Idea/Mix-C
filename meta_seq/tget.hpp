
#ifndef xpack_meta_seq_tget
#define xpack_meta_seq_tget
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::meta_seq_tget
        #include"define/base_type.hpp"
        #include"meta_seq/tnull.hpp"
        #include"meta_seq/tlist.hpp"
        #include"meta_ctr/cif.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::meta_seq_tget{
        using namespace inc;

        template<class tlist, ixx index> struct tget;
        template<ixx index, class first, class ... args>
        struct tget<tlist<first, args...>, index>{
            using item = typename cif<
                index == 0 || -index == 1 + sizeof...(args),
                first,
                typename tget<tlist<args...>, (index < 0 ? index : index - 1)>::item
            >::result;
        };

        template<ixx index>
        struct tget<tlist<>, index>{
            using item = tnull;
        };
    }

#endif

namespace xuser::inc{
    using ::mixc::meta_seq_tget::tget;
}
