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
        xgc(core, xtmpl(item))
            xgc_fields(
                xpri(ptr, item *),
                xpub(length, uxx)
            );

            core() : core(& empty, 0) {}
            core(core const &) = default;
            core(item const * ptr, uxx length) : 
                ptr((item *)ptr), length(length) {
            }

            core(mixc::static_string_holder<item> holder){
                ptr = holder.ptr();
                length = holder.length();
            }

            operator item *(){
                return ptr;
            }

            operator const item *() const {
                return ptr;
            }

            auto backward(uxx value) const {
                auto tmp = this[0];
                tmp.ptr += value;
                tmp.length -= uxx(value);
                return tmp;
            }

            auto forward(uxx value) const {
                return backward(
                    uxx(-ixx(value))
                );
            }

            auto shorten(uxx length) const {
                auto tmp = this[0];
                tmp.length -= length;
                return tmp;
            }

            auto elongate(uxx length) const {
                return shorten(
                    uxx(-ixx(length))
                );
            }
        private:
            inline static item empty = item(0);
        xgc_end();

        template<class item, class final>
        struct cxx : core<item> {
            using core<item>::core;
            using core<item>::length;

            cxx(core<item> const & self) : 
                core<item>(self) {
            }

            final backward(uxx value) const {
                return core<item>::backward(value);
            }

            final forward(uxx value) const {
                return core<item>::forward(value);
            }

            final shorten(uxx length) const {
                return core<item>::shorten(length);
            }

            final elongate(uxx length) const {
                return core<item>::elongate(length);
            }
        };
    }

    #define xc08(str)      xsv(char, "\0\0\0\0\0\0\0\0", str)
#endif

#define xusing_lang_cxx     mixc::lang_cxx
