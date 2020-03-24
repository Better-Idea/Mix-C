#ifndef xpack_lang_ascii
#define xpack_lang_ascii

    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lang_ascii
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lang_ascii{
        xgc(core)
            xgc_fields(
                xpri(data, char)
            );
        public:
            core() = default;
            core(core const &) = default;
            core(char value) : data(value){}

            static constexpr uxx distance_of_lower_and_upper = u'a' - u'A';
        xgc_end();
    }

    namespace mixc::lang_ascii::inc{
        template<class final = core>
        struct ascii : core {
            using core::core;

            // final () const {
            //     return core::();
            // }
        };
    }
#endif

namespace xuser::inc{
    using namespace mixc::lang_ascii::inc;
}

#define xusing_lang_ascii     mixc::lang_ascii::inc
