#ifndef xpack_meta_ctr_cif
#define xpack_meta_ctr_cif
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::meta_ctr_cif::inc
#include"mixc.hpp"
#pragma pop_macro("xuser")

namespace mixc::meta_ctr_cif{
    template<bool condition, class if_true, class if_false>
    struct meta{
        using result = if_true;
    };

    template<class if_true, class if_false>
    struct meta<false, if_true, if_false>{
        using result = if_false;
    };

    template<bool c1, class a1, bool c2>
    struct ei{
        template<class else_if_type>
        struct select{
        private:
            using t = typename meta<c1, a1, else_if_type>::result;
        public:
            template<bool type_else_if_true>
            using cei = ei<
                c1 | c2, t, type_else_if_true
            >;

            template<class type_else>
            using ces = typename meta<c1 | c2, t, type_else>::result;
        };
    };

    template<bool if_condition>
    struct cif{
        template<class type_if_true>
        struct select{
            template<bool type_else_if_true>
            using cei = ei<
                if_condition, type_if_true, type_else_if_true
            >;

            template<class type_else>
            using ces = typename meta<if_condition, type_if_true, type_else>::result;
        };
    };
}

#endif

xexport(mixc::meta_ctr_cif::cif)
