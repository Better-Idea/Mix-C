#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_trim_start
#define xpack_lang_cxx_trim_start
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_trim_start
        #include"define/base_type.hpp"
        #include"interface/can_alloc.hpp"
        #include"interface/initializer_list.hpp"
        #include"lang/cxx/clone.hpp"
        #include"lang/cxx/index_of_first_miss.hpp"
        #include"lang/cxx.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_trim_start{
        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto trim_start(inc::initializer_list<item> values, inc::can_alloc<item> alloc) const {
                auto token  = the_t(values.begin(), values.begin());
                auto offset = the.index_of_first_miss(values);

                if (offset == not_exist){
                    return the_t();
                }
                if (alloc == nullptr){
                    return the.backward(offset);
                }
                else {
                    return the.backward(offset).clone(alloc);
                }
            }
        };
    }
#endif

namespace mixc::lang_cxx_trim_start::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final trim_start(inc::initializer_list<item> values, inc::can_alloc<item> alloc = nullptr) const {
            return the.trim_start(values, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_trim_start::xuser
