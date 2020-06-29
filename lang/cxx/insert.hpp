#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_insert
#define xpack_lang_cxx_insert
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
    #undef  xuser
    #define xuser mixc::lang_cxx_insert
    #include"define/base_type.hpp"
    #include"interface/can_alloc.hpp"
    #include"lang/cxx.hpp"
    #include"macro/xindex_rollback.hpp"
    #include"memop/copy.hpp"

    namespace xuser{
        template<class item>
        struct core : inc::cxx<item> {
            using base_t = inc::cxx<item>;
            using base_t::base_t;
            using the_t = core<item>;

            core(base_t const & self) : 
                base_t(self){}

            auto insert(ixx index, the_t value, inc::can_alloc<item> alloc) const {
                xindex_rollback(the.length(), index, + 1);
                uxx                     target_length = the.length() + value.length();
                the_t                   r { alloc(target_length), target_length };
                inc::copy_with_operator(r, the, index);
                inc::copy_with_operator(r.backward(index), value, value.length());
                inc::copy_with_operator(r.backward(index + value.length()), the.backward(index), the.length() - index);
                return r;
            }
        };
    }

    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace mixc::lang_cxx_insert::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final insert(ixx index, final value, inc::can_alloc<item> alloc) const {
            return the.insert(index, value, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_insert::xuser
