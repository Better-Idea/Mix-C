#ifndef xpack_lang_cxx_ignore_case
#define xpack_lang_cxx_ignore_case
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lang_cxx_ignore_case
        #include"define/base_type.hpp"
        #include"lang/wxx/to_lower.hpp"
        #include"lang/wxx.hpp"
        #include"macro/xcmp.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_ignore_case{
        template<class item>
        inline auto const & ignore_case = 
            []xcmp(item) {
                return 
                    inc::wxx<item>(left).to_lower() - 
                    inc::wxx<item>(right).to_lower();
            };
    }

#endif

namespace xuser::inc{
    using namespace mixc::lang_cxx_ignore_case;
}
