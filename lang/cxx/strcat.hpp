#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_strcat
#define xpack_lang_cxx_strcat
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_strcat
        #include"define/base_type.hpp"
        #include"lang/private/layout_args.hpp"
        #include"lang/cxx.hpp"
        #include"memory/alloc_callback.hpp"
        #include"meta/is_same.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_strcat{
        

        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            auto strcat(the_t * list, uxx length, inc::alloc_callback<item> alloc) const {
                uxx total_length = 0;

                for(uxx i = 0; i < length; i++){
                    total_length += list[i].length();
                }

                item * ptr = alloc(total_length);
                
                for(uxx i = 0; i < length; i++){
                    list[i].clone([&](uxx length){
                        auto mem = ptr;
                        ptr     += length;
                        return mem;
                    });
                }
                return the_t(ptr, total_length);
            }

            template<class ... args>
            auto strcat(args const & ... list) const {
                constexpr uxx length = sizeof...(args) - 1;
                the_t         buf[length];
                auto          alloc = inc::layout_args<the_t, 0>(buf, list...);
                static_assert(inc::is_same<inc::alloc_callback<item>, decltype(alloc)>);
                return strcat(buf, length, alloc);
            }
        };
    }
#endif

namespace mixc::lang_cxx_strcat::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item>{
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        template<class a0, class ... args>
        final strcat(a0 const & first, args const & ... list) const {
            return the.strcat(the, first, list...);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_strcat::xuser
