#ifndef xpack_lang_cxx
#define xpack_lang_cxx
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::lang_cxx
        #include"define/base_type.hpp"
        #include"macro/xgc.hpp"
        #include"macro/xprop.hpp"
        #include"macro/xsv.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx{
        template<class final, class item>
        struct cxx{
        private:
            inline static item empty = item(0);
        public:
            xgc_fields(
                xiam(cxx<final, item>),
                xpro(ptr,   item *),
                xpro(count, uxx)
            );

            cxx() : 
                cxx(& empty, 0) {}

            cxx(cxx const &) = default;

            template<class final_t>
            cxx(cxx<final_t, item> const & self) : 
                cxx((the_t &)self){
            }

            template<class type>
            cxx(type const * ptr, uxx count) : 
                ptr((item *)ptr), count(count) {
                static_assert(sizeof(type) == sizeof(item));
            }

            cxx(inc::static_string_holder<item> holder) : 
                cxx(holder.ptr(), holder.length()){
            }

            item & operator [](uxx index) const {
                return the.ptr[index];
            }

            operator item *(){
                return the.ptr;
            }

            operator const item *() const {
                return the.ptr;
            }

            final backward(uxx value) const {
                auto tmp = thex;
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
                auto tmp = thex;
                tmp.count -= count;
                return tmp;
            }

            final elongate(uxx count) const {
                return shorten(
                    uxx(-ixx(count))
                );
            }

        public:
            xpubget_pubset(length, uxx){
                xr{ return the.count; }
                xw{ the.count = value; }
            };

            xpubget(is_empty, bool){
                return length() == 0;
            };
        };
    }

    #if xos64
        #define xc08(str)      xsv(char, "0123456789abcdef", str)
    #elif xos32
        #define xc08(str)      xsv(char, "01234567", str)
    #endif
#endif

#define xusing_lang_cxx     ::mixc::lang_cxx
