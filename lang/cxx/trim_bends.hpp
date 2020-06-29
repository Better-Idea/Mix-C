#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_trim_bends
#define xpack_lang_cxx_trim_bends
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
    #undef  xuser
    #define xuser mixc::lang_cxx_trim_bends
    #include"define/base_type.hpp"
    #include"interface/can_alloc.hpp"
    #include"interface/initializer_list.hpp"
    #include"lang/cxx/clone.hpp"
    #include"lang/cxx/index_of_first_miss.hpp"
    #include"lang/cxx/index_of_last_miss.hpp"
    #include"lang/cxx.hpp"
    #include"meta/is_same.hpp"

    namespace xuser{
        template<class item>
        struct core : inc::cxx<item> {
            using base_t = inc::cxx<item>;
            using base_t::base_t;
            using the_t = core<item>;

            core(base_t const & self) : 
                base_t(self){}

            auto trim_bends(inc::initializer_list<item> values, inc::can_alloc<item> alloc) const {
                auto token  = the_t(values.begin(), values.size());
                auto offset = the.index_of_first_miss(values);

                if (offset == not_exist){
                    return the_t();
                }

                auto temp = the.backward(offset);
                temp.length(
                    temp.index_of_last_miss(values)
                );

                if (alloc != nullptr){
                    temp = temp.clone(alloc);
                }
                return temp;
            }
        };
    }

    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")
#endif

namespace mixc::lang_cxx_trim_bends::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final trim_bends(item value, inc::can_alloc<item> alloc = nullptr) const {
            return the.trim_bends({ value }, alloc);
        }

        final trim_bends(inc::initializer_list<item> values, inc::can_alloc<item> alloc = nullptr) const {
            return the.trim_bends(values, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_trim_bends::xuser
