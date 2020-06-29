#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_clone
#define xpack_lang_cxx_clone
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
    #undef  xuser
    #define xuser mixc::lang_cxx_clone
    #include"define/base_type.hpp"
    #include"interface/can_alloc.hpp"
    #include"lang/cxx.hpp"
    #include"macro/xgc.hpp"
    #include"memop/copy.hpp"
    

    namespace xuser{
        template<class item>
        struct core : inc::cxx<item> {
            using base_t = inc::cxx<item>;
            using base_t::base_t;
            using the_t = core<item>;

            core(base_t const & self) : 
                base_t(self){}

            auto clone(inc::can_alloc<item> alloc) const {
                the_t  r{ alloc(the.length()), the.length() };
                inc::copy_with_operator(r, the, the.length());
                return r;
            }
        };
    }

    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace mixc::lang_cxx_clone::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final clone(inc::can_alloc<item> alloc) const {
            return the.clone(alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_clone::xuser
