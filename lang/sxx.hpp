#ifndef xpack_lang_sxx
#define xpack_lang_sxx
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lang_sxx
        #include"define/base_type.hpp"
        #include"gc/private/ref.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xsv.hpp"
    #pragma pop_macro("xuser")

    #define xsv08(str)  xsv(mixc::s08, char, "01234567", str)

    namespace mixc::lang_sxx{
        template<class item>
        struct sxx :  {
            using base = mixc::inner_gc::meta<sxx, item, dummy_t, true>;

            item operator[](uxx index) const {
                return base::operator[](index);
            }
        };

        template<class item>
        xgc(core, xtmpl(item), xpub())
            xgc_fields();
        public:
            core() = default;
            core(core const &) = default;

            template<class expand>
            core(expand const & self) : 
                core((core &)(expand &)self) {
            }

            auto () const {}
        xgc_end();
    }

    namespace mixc::lang_sxx::inc{
        template<class item, class final = core<item>>
        struct sxx : core<item> {
            using core<item>::core;

            final () const {
                return core<item>::();
            }
        };
    }
#endif

namespace xuser::inc{
    using namespace mixc::lang_sxx::inc;
}

#define xusing_lang_sxx     mixc::lang_sxx::inc
