#ifndef xpack_lang_cxx
#define xpack_lang_cxx
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lang_cxx
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xsv.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx{
        template<class item>
        xgc(core)
            xgc_fields(
                xpro(ptr, item *),
                xpro(count, uxx)
            );

            core() : core(& empty, 0) {}
            core(core const &) = default;
            core(item const * ptr, uxx count) : 
                ptr((item *)ptr), count(count) {
            }

            core(mixc::static_string_holder<item> holder){
                ptr = holder.ptr();
                count = holder.length();
            }
        protected:
            inline static item empty = item(0);
        xgc_end();

        template<class final, class item>
        struct cxx : core<item> {
            using core<item>::core;
            using the_t = core<item>;

            auto & operator [](uxx index) const {
                return core<item>::ptr[index];
            }

            operator item *(){
                return core<item>::ptr;
            }

            operator const item *() const {
                return core<item>::ptr;
            }

            final backward(uxx value) const {
                auto tmp = the;
                tmp.ptr += value;
                tmp.count -= uxx(value);
                return tmp;
            }

            final forward(uxx value) const {
                return backward(
                    uxx(-ixx(value))
                );
            }

            final shorten(uxx count) const {
                auto tmp = the;
                tmp.count -= count;
                return tmp;
            }

            final elongate(uxx count) const {
                return shorten(
                    uxx(-ixx(count))
                );
            }

            auto length() const {
                return core<item>::count;
            }
        };
    }

    #define xc08(str)      xsv(char, "\0\0\0\0\0\0\0\0", str)
#endif

#define xusing_lang_cxx     mixc::lang_cxx
