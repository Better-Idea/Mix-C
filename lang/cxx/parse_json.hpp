#ifndef xusing_lang_cxx
    #include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_json
#define xpack_lang_cxx_json
    #pragma push_macro("xuser")
    #pragma push_macro("xusing_lang_cxx")
        #undef  xuser
        #define xuser mixc::lang_cxx_json
        #include"define/base_type.hpp"
        #include"dumb/disable_copy.hpp"
        #include"interface/can_alloc.hpp"
        #include"lang/wxx/is_whitespace.hpp"
        #include"lang/wxx/is_hex.hpp"
        #include"lang/wxx.hpp"
        #include"lang/cxx/is_starts_with.hpp"
        #include"lang/cxx/compare_fastly.hpp"
        #include"lang/cxx/parse.hpp"
        #include"lang/cxx.hpp"
        #include"macro/xdebug_fail.hpp"
        #include"memory/new.hpp"
        #include"meta/is_same.hpp"
    #pragma pop_macro("xusing_lang_cxx")
    #pragma pop_macro("xuser")

    namespace mixc::lang_cxx_json {
        enum class json_type_t {
            json_object,
            json_string,
            json_array,
            json_number,
        };

        namespace json_type {
            constexpr json_type_t json_object = json_type_t::json_object;
            constexpr json_type_t json_string = json_type_t::json_string;
            constexpr json_type_t json_array  = json_type_t::json_array;
            constexpr json_type_t json_number = json_type_t::json_number;
        }

        template<class index_t> union  json_value_t;
        template<class index_t> struct json_list_t;
        template<class index_t> struct json_pair_t;
        template<class index_t> struct json_string_t;
        template<class index_t> using  json_object_t = json_list_t<json_pair_t<index_t>>;
        template<class index_t> using  json_array_t  = json_list_t<json_value_t<index_t>>;

        constexpr uxx bits_of_type_fields = 2;

        template<class index_t>
        union json_value_t {
            struct {
                u64                     type   : bits_of_type_fields;
                u64                     number : sizeof(uxx) * 8 - bits_of_type_fields;
            };
            json_string_t<index_t>      string;
            json_object_t<index_t> *    object;
            json_array_t<index_t>  *    array;
            f64                         real;
        };

        template<class index_t>
        struct json_string_t {
            index_t                     type   : bits_of_type_fields;
            index_t                     offset : sizeof(index_t) * 8 - bits_of_type_fields;
            index_t                     length;
        };

        template<class index_t>
        struct json_pair_t {
            json_string_t<index_t>      name;
            json_value_t<index_t>       value;
        };

        template<class item_t>
        struct json_list_t {
            uxx                         length  = 0;
            item_t & operator[](uxx index) {
                return ((item_t *)this)[1 + index];
            }
        };

        using namespace json_type;

        // template<class item_t> struct core;
        // using item_t = char;
        // template<> struct core<item_t> : inc::cxx<item_t> {

        template<class item_t>
        struct core : inc::cxx<item_t> {
            using inc::cxx<item_t>::cxx;
            using the_t = core<item_t>;

            template<class base_t>
            core(base_t const & self) : inc::cxx<item_t>(self){}

            struct any{
                any(u08p buffer) : 
                    begin(buffer), end(buffer){}

                template<class object>
                object * array(uxx length) {
                    object * ptr = (object *)end;
                    end          = u08p(ptr + length);
                    // uxx cost     = end - begin;
                    // xdebug(true, cost);
                    return ptr;
                }

                template<class object>
                operator object * (){
                    auto ptr = array<object>(1);
                    return new (ptr) object();
                }
            private:
                u08p begin;
                u08p end;
            };

            auto skip_whitespace(uxx & i) {
                while(inc::wxx<item_t>(the[i]).is_whitespace() and i < the.length()) {
                    i++;
                }
            }

            template<class index_t>
            json_value_t<index_t> parse_value(uxx & i, any & alloc) {
                json_value_t<index_t> value;
                the_t candidate[] = {
                    the_t{ "true",    4 },
                    the_t{ "null",    4 },
                    the_t{ "false",   5 },
                };

                if (the.skip_whitespace(i); 
                    the[i] == '{') {
                    value.object = the.template parse_object<index_t>(i, alloc);
                    value.type   = u08(json_object);
                }
                else if (the[i] == '[') {
                    value.array  = the.template parse_array<index_t>(i, alloc);
                    value.type   = u08(json_array);
                }
                else if (the[i] == '\"') {
                    value.string = the.template parse_string<index_t>(i);
                    value.type   = u08(json_string);
                }
                else {
                    auto cur = the.backward(i);
                    auto ii = 0;

                    for(ii = 0; ii < 3; ii++) {
                        if (cur.is_starts_with(candidate[ii])) {
                            i           += candidate[ii].length();
                            value.number = ii == 0; // is "true"
                            value.type   = u08(json_number);
                            break;
                        }
                    }

                    if (ii == 3) {
                        value.real = the.parse_number(i);
                        value.type = u08(json_number); // set this field after value.real
                    }
                }
                return value;
            }

            template<class jlist_t, class jitem_t, class index_t>
            auto parse_list(uxx & i, any & alloc, item_t end_char) {
                jitem_t   c;
                jlist_t * obj    = alloc;
                jitem_t * ptr    = alloc.template array<jitem_t>(0); // get current memory head

                if (the[++i] == end_char) {
                    i           += 1; // skip '}' or ']'
                    return obj;
                }
                
                while(true) {
                    if (auto over_range = i == the.length(); over_range) {
                        xdebug_fail(over_range);
                        break;
                    }
                    if constexpr (inc::is_same<jitem_t, json_pair_t<index_t>>) {
                        c.name   = the.template parse_string<index_t>(i);
                        the.skip_whitespace(i);
                        xdebug_fail(the[i] != ':');
                        i       += 1;
                        c.value  = the.template parse_value<index_t>(i, alloc);
                    }
                    else {
                        c        = the.template parse_value<index_t>(i, alloc);
                    }

                    ptr[obj->length++] = c;

                    the.skip_whitespace(i);

                    if (the[i] == ',') {
                        i       += 1; // skip ','
                    }
                    else if (the[i] == end_char) {
                        i       += 1;
                        break;
                    }
                }

                alloc.template array<jitem_t>(obj->length); // real cost
                return obj;
            }

            template<class index_t>
            auto parse_object(uxx & i, any & alloc) {
                return the.template parse_list<json_object_t<index_t>, json_pair_t<index_t>, index_t>(i, alloc, '}');
            }

            template<class index_t>
            auto parse_array(uxx & i, any & alloc) {
                return the.template parse_list<json_array_t<index_t>, json_value_t<index_t>, index_t>(i, alloc, ']');
            }

            template<class index_t>
            auto parse_string(uxx & i) {
                using str_t = json_string_t<index_t>;
                str_t str;
                i         += 1; // skip '\"'
                str.offset = i;

                for(bool over = false, need_cvt = false; not over; i++) {
                    xdebug_fail(i == the.length());

                    if (need_cvt) switch (need_cvt = false; the[i]) {
                    case '\"': 
                    case '\\':
                    case '/':
                    case 'b':
                    case 'f':
                    case 'n':
                    case 'r':
                    case 't':
                        continue;
                    case 'u':
                        xdebug_fail(i + 4 >= the.length());
                        for (uxx e = i + 4; i < e; i++) {
                            xdebug_fail(not inc::wxx<item_t>(the[i]).is_hex());
                        }
                        continue;
                    default:
                        xdebug_fail(false);
                        continue;
                    }

                    switch(the[i]) {
                    case '\"': over     = true; break;
                    case '\\': need_cvt = 1;    break;
                    }
                }

                str.length = i - str.offset - 1;
                return str;
            }

            f64 parse_number(uxx & i) {
                the.skip_whitespace(i);
                auto r = the.backward(i).template parse<f64>();
                i      = r.is_parse_error() ? i + r.index_of_error() : the.length();
                xdebug_fail(r.index_of_error() == 0);
                return f64(r);
            }

            template<class final, class index_t>
            struct json_t : inc::disable_copy {
                using jval = json_value_t<index_t>;
                json_t(){}
                json_t(final json, jval value) :
                    json(json), value(value){
                }

                operator f64 () const {
                    return origin().real;
                }

                operator bool() const {
                    return origin().number != 0;
                }

                operator final() const {
                    auto str = origin().string;
                    return json.backward(str.offset).length(str.length);
                }

                json_t operator [](final index) const {
                    auto & obj = origin().object[0];

                    for(uxx i = 0; i < obj.length; i++) {
                        auto str  = obj[i].name;
                        auto name = json.backward(str.offset).length(str.length);

                        if (inc::cxx<item_t>(index).compare_fastly(name) == 0) {
                            return { json, obj[i].value) };
                        }
                    }
                    return {};
                }

                json_t operator[](uxx index) const {
                    return { json, origin().array[0][index] };
                }

                uxx length() const {
                    auto list = origin().array;

                    if (value.type == u08(json_object) or 
                        value.type == u08(json_array) or list == nullptr) {
                        return 0;
                    }
                    return list.length;
                }
            private:
                jval origin() const {
                    jval temp = value;
                    temp.type = 0;
                    return temp;
                }
                final json;
                jval  value;
            };

            template<class final, class index_t>
            json_t<final, index_t> parse_json(inc::can_alloc<byte> alloc) {
                uxx i   = 0;
                any mem = alloc(the.length() * 5 + 1);
                return { the, the.template parse_value<index_t>(i, mem) };
            }
        };
    }
#endif

namespace mixc::lang_cxx_json::xuser {
    template<class final, class item_t>
    struct cxx : xusing_lang_cxx::cxx<final, item_t> {
        using xusing_lang_cxx::cxx<final, item_t>::cxx;
        using the_t = core<item_t>;

        template<class index_t = u32>
        auto parse_json(inc::can_alloc<byte> alloc) {
            return the.template parse_json<final, index_t>(alloc);
        }
    };
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx  ::mixc::lang_cxx_json::xuser
