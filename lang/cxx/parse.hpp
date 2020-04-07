#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_parse
#define xpack_lang_cxx_parse
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xusing_lang_cxx
        #undef  xuser
        #define xuser mixc::lang_cxx_parse
        #include"define/base_type.hpp"
        #include"define/platform.hpp"
        #include"docker/transmitter.hpp"
        #include"lang/cxx.hpp"
        #include"math/numeration.hpp"
        #include"meta/unsigned_type.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_parse{
        constexpr u08 lut_error = 0xff;
        constexpr u08 lut[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, lut_error, lut_error, lut_error, lut_error, lut_error, lut_error, lut_error, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, lut_error, lut_error, lut_error, lut_error, lut_error, lut_error, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, lut_error, lut_error, lut_error, lut_error, lut_error, };

        template<class target>
        struct parse_result{
            template<class a>
            parse_result(parse_result<a> self) : 
                parse_result(target(self.value), self.error_index) {}

            parse_result(target v, uxx i) : 
                value(v), error_index(i){}

            parse_result(target v) : 
                value(v), error_index(not_exist){}

            operator target & (){
                return value;
            }

            operator const target & () const {
                return value;
            }

            bool is_parse_error() const {
                return error_index != not_exist;
            }

            uxx index_of_error() const {
                return error_index;
            }
        private:
            template<class a> friend struct parse_result;
            target value; 
            uxx  error_index;
        };

        // using item = char;
        // template<class item> struct core;
        // template<>
        // struct core<item> : inc::cxx<item> {

        template<class item>
        struct core : inc::cxx<item> {
            using inc::cxx<item>::cxx;
            using the_t = core<item>;

            template<class target>
            auto parse(uxx base) const {
                target   value  = 0;
                bool   is_neg = false;
                item * cur    = the;
                item * begin  = the;
                item * end    = cur + the.length();

                if (cur[0] == '+'){
                    cur++;
                }
                else if (cur[0] == '-'){
                    cur++;
                    is_neg = true;
                }
                for(; cur < end; cur++){
                    constexpr uxx mask = 0x4f; // mask 要大于 'z' - '0'
                    static_assert(mask <= sizeof(lut) / sizeof(lut[0]));
                    static_assert(mask > 'z' - '0');

                    uxx dis = uxx(cur[0] - '0');
                    uxx v   = lut[dis & mask];

                    if (dis >= mask or v == lut_error){
                        return parse_result<target>(0, cur - begin);
                    }
                    else{
                        value = value * base + v;
                    }
                }

                if (is_neg){
                    value = target(0) - value;
                }
                return parse_result<target>(value);
            }
        };
    }
#endif

namespace mixc::lang_cxx_parse::xuser{
    template<class final, class item>
    struct cxx : xusing_lang_cxx::cxx<final, item>{
        using xusing_lang_cxx::cxx<final, item>::cxx;
        using the_t = core<item>;

        template<class target>
        parse_result<target> parse() const {
            return parse<target>(inc::dec);
        }

        template<class target>
        parse_result<target> parse(inc::numeration base) const {
            // TODO: floating parse ======================================================
            if constexpr (is_64bit or sizeof(target) <= sizeof(uxx)){
                return the.template parse<uxx>(uxx(base));
            }
            else if constexpr (is_32bit){
                return the.template parse<unsigned_type<target>>(uxx(base));
            }
        }
    };
}

#include"math/numeration.hpp"

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_parse::xuser
