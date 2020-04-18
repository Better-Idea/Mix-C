#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_slice
#define xpack_lang_cxx_slice
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
    #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_slice
        #include"define/base_type.hpp"
        #include"interface/can_alloc.hpp"
        #include"lang/cxx.hpp"
        #include"macro/xdebug_fail.hpp"
        #include"macro/xindex_rollback.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_slice{
        // using item = char;
        // template<class item> struct core;
        // template<>
        // struct core<item> : inc::cxx<item> {

        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            void rollback(ixx & start, ixx & end) const {
                xindex_rollback(the.length(), start);
                xindex_rollback(the.length(), end);

                xdebug_fail(start >= the.length());
                xdebug_fail(end   >= the.length());
            }

            auto slice(ixx start, ixx end) const {
                rollback(start, end);
                xdebug_fail(start > end);
                return the.backward(start).length(end - start + 1);
            }

            auto slice(ixx start, ixx end, inc::can_alloc<item> alloc) const {
                rollback(start, end);

                uxx    target_length;
                item * buf;
                item * temp;

                if (start <= end) {
                    target_length   = uxx(end - start + 1);
                    temp            = buf = alloc(target_length);
                    while(start <= end){
                        temp[0]     = the[start];
                        temp       += 1;
                        start      += 1;
                    }
                }
                else{
                    target_length   = uxx(start - end + 1);
                    temp            = buf = alloc(target_length);
                    while(start >= end){
                        temp[0]     = the[start];
                        temp       += 1;
                        start      -= 1;
                    }
                }
                return the_t(buf, target_length);
            }
        };
    }
#endif

namespace mixc::lang_cxx_slice::xuser {
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item> {
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        final slice(ixx start, ixx end) const {
            return the.slice(start, end);
        }

        final slice(ixx start, inc::can_alloc<item> alloc) const {
            return slice(start, -1, alloc);
        }

        final slice(ixx start, ixx end, inc::can_alloc<item> alloc) const {
            return the.slice(start, end, alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_slice::xuser
