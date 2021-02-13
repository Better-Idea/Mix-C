#ifndef xusing_lang_cxx
#include"lang/private/cxx.hpp"
#endif

#ifndef xpack_lang_cxx_parse_json
#define xpack_lang_cxx_parse_json
#pragma push_macro("xuser")
#pragma push_macro("xusing_lang_cxx")
#undef  xusing_lang_cxx
#undef  xuser
#define xuser mixc::lang_cxx_parse_json::inc
#include"define/base_type.hpp"
#include"interface/can_alloc.hpp"
#include"macro/xnew.hpp"
#include"math/expr10.hpp"
#include"math/exp10.hpp"
#include"meta/is_number.hpp"
#include"meta/more_fit.hpp"
#include"lang/cxx/compare_fastly.hpp"
#include"lang/cxx.hpp"
#pragma pop_macro("xusing_lang_cxx")
#pragma pop_macro("xuser")

namespace mixc::lang_cxx_parse_json{
    template<class item_t> struct json_object;
    template<class item_t> struct json_array;

    enum class json_type_t : uxx{
        jobject,
        jarray,
        jfloat,
        jinteger,
        jstring,
        unknwon,
    };

    enum class json_parse_result_t : uxx{
        success,
        depth_overflow,
        terminator_missing,
        terminator_mismatch,
        colon_mismatch,
        unexpected_termination,
        unexpected_key_format,
        unexpected_value_format,
        remainder_comma,
        remainder_terminator,
    };

    template<class item_t>
    union json_value{
        item_t const        * s;
        f64                   f;
        u64                   u;
        i64                   i;
        json_array<item_t>  * a;
        json_object<item_t> * o = nullptr;
    };

    template<class item_t, class next_t>
    struct json_struct{
        json_value<item_t>  value;

        json_type_t type() {
            return json_type_t(ptype);
        }
        
        void type(json_type_t value) {
            ptype = uxx(value);
        }

        next_t * next(){
            return (next_t *)(uxx(pnext) & uxx(~0x7));
        }

        void next(next_t * value){
            pnext = (next_t *)(uxx(value) | ptype);
        }

    private:
        union{
            uxx             ptype : 3;
            next_t *        pnext = nullptr;
        };
    };

    template<class item_t>
    struct json_array : json_struct<item_t, json_array<item_t>>{};

    template<class item_t>
    struct json_object : json_struct<item_t, json_object<item_t>>{
        item_t const *      key;

        json_object() :
            key((item_t *)inc::cxx<item_t>{}){
        }
    };

    template<class item_t>
    using json_arrayp   = json_array<item_t> *;

    template<class item_t>
    using json_objectp  = json_object<item_t> *;

    template<class item_t>
    using json_valuep   = json_value<item_t> *;

    template<class item_t>
    xstruct(
        xtmpl(json, item_t),
        xprif(ptr, voidp)
    )
    private:
        using jap       = json_arrayp<item_t>;
        using jop       = json_objectp<item_t>;
        using jvp       = json_valuep<item_t>;
        using final_t   = the_t;
    public:
        json(voidp ptr = nullptr) : ptr(ptr){}

        json<item_t> operator[](item_t const * name) const {
            return the[inc::cxx<item_t>{name}];
        }

        json<item_t> operator[](inc::cxx<item_t> name) const {
            for(jop object = jop(ptr)->value.o;;){
                if (name == object->key){
                    return object;
                }
                if (object = object->next(); object == nullptr){
                    return nullptr;
                }
            }
        }

        template<inc::is_number number_t>
        json<item_t> operator[](number_t index) const {
            jap object  = jop(ptr)->value.a;
            uxx i       = 0;

            while(true){
                if (i == index){
                    return object;
                }
                if (i += 1, object = object->next(); object == nullptr){
                    return nullptr;
                }
            }
        }

        operator f64 () const {
            return jop(ptr)->value.f;
        }

        operator i64 () const {
            return jop(ptr)->value.i;
        }

        operator u64 () const {
            return jop(ptr)->value.u;
        }

        operator inc::cxx<item_t> () const {
            return jop(ptr)->value.s;
        }

        operator bool() const {
            return jop(ptr)->value.i != 0;
        }

        template<class type_t>
        operator type_t () const {
            using result_t  = inc::more_fit<type_t, f64, i64, u64, inc::cxx<item_t>>;
            using fit_t     = typename result_t::type;
            //return (type_t)(fit_t)(the); // msvc 不支持
            return (type_t)(operator fit_t());
        }

        xpubgetx(length, uxx){
            uxx len     = 0;
            jop object  = jop(ptr)->value.o;

            while(object != nullptr){
                object  = object->next();
                len    += 1;
            }
            return len;
        }

        xpubgetx(type, json_type_t){
            return jop(ptr)->type();
        }
    $

    template<class item_t>
    xstruct(
        xtmpl(jsonx, item_t),
        xpubb(json<item_t>),
        xprif(pparse_result, json_parse_result_t),
        xprif(plocation_of_error, item_t const *)
    )
        using jarray_t = json_array<item_t>;
        using final_t    = the_t;
        using json<item_t>::json;

        jsonx(json_parse_result_t parse_result, item_t const * location_of_error) : 
            pparse_result(parse_result), plocation_of_error(location_of_error){
        }
    public:
        xpubget(parse_result)
        xpubget(location_of_error)
    $

    template<class item_t>
    inline item_t const * skip_whitespace(item_t const * json_string){
        char whitespace[] = {
            char(0x09), char(0x0a), char(0x0b), char(0x0c), char(0x0d), char(0x20),
        };

        for(; json_string[0] != '\0'; json_string++){
            for(uxx i = 0; ; i++){
                if (i == sizeof(whitespace) / sizeof(whitespace[0])){
                    return json_string;
                }
                if (json_string[0] == whitespace[i]){
                    break;
                }
            }
        }
        return json_string;
    }

    template<class item_t>
    inline bool start_with(item_t const * json_string, asciis value){
        while(json_string[0] == (item_t)value[0]){
            json_string        += 1;
            value              += 1;
        }
        return value[0] == '\0';
    }

    template<class item_t>
    inline item_t const * parse_string(item_t ** buffer, item_t const * json_string){
        item_t * buf            = (item_t *)(* buffer);

        for(; json_string[0] != '\0' and json_string[0] != '\"'; json_string++){
            if (json_string[0] != '\\'){
                buf[0]          = json_string[0];
                buf            += 1;
                continue;
            }

            if (json_string += 1; json_string[0] == '\0'){
                return json_string;
            }

            switch(json_string[0]){
            case '\"': buf[0] = '\"'; buf += 1; break;
            case '\\': buf[0] = '\\'; buf += 1; break;
            case '/' : buf[0] = '/' ; buf += 1; break;
            case 'b' : buf[0] = '\b'; buf += 1; break;
            case 'f' : buf[0] = '\f'; buf += 1; break;
            case 'n' : buf[0] = '\n'; buf += 1; break;
            case 'r' : buf[0] = '\r'; buf += 1; break;
            case 't' : buf[0] = '\t'; buf += 1; break;
            case 'u' : 
                // to convert =============================================
                break;
            default:
                return json_string; // 未知转义符
            }
        }
        buf[0]                  = '\0';
        buf                    += 1;
        buffer[0]               = static_cast<item_t *>(buf);
        return json_string;
    }

    template<class item_t>
    inline item_t const * parse_number(void * buffer, json_type_t * type, item_t const * json_string){
        auto is_neg             = [&](){
            if (json_string[0] == '-'){
                json_string    += 1;
                return true;
            }
            if (json_string[0] == '+'){
                json_string    += 1;
            }
            return false;
        };

        auto fetch_number       = [&](uxx * ptr = nullptr){
            u64 value           = 0;
            uxx idx             = 0;

            if (ptr == nullptr){
                ptr             = & idx;
            }

            // TODO：超过精度时转为浮点类型
            for(uxx & i = ptr[0]; i < 19; json_string++, i++){
                if (json_string[0] >= '0' and json_string[0] <= '9'){
                    value       = value * 10 + json_string[0] - '0';
                }
                else if (i == 0){
                    break;
                }
                else{
                    return value;
                }
            }

            type[0]             = json_type_t::unknwon;
            return value;
        };

        type[0]                 = json_type_t::jinteger;
        auto is_neg_real        = is_neg();
        auto is_neg_exp         = false;
        auto i                  = uxx(0);
        auto exp                = uxx(0);
        auto real               = fetch_number();
        auto deci               = 0.0;
        auto & u                = u64p(buffer)[0];
        auto & f                = f64p(buffer)[0];

        if (type[0] == json_type_t::unknwon){
            return json_string;
        }
        if (json_string[0] == '.'){
            type[0]             = json_type_t::jfloat;
            json_string        += 1;
            deci                = static_cast<f64>(fetch_number(& i));
            deci                = inc::expr10_unsafe(i) * deci + real;
        }
        if (json_string[0] == 'e' or json_string[0] == 'E'){
            json_string        += 1;
            is_neg_exp          = is_neg();
            exp                 = fetch_number(& i);

            if (exp > 307){ // max exp
                type[0]         = json_type_t::unknwon;
                return json_string;
            }
            deci               *= is_neg_exp ? inc::expr10(exp) : inc::exp10(exp);
        }

        if (type[0] == json_type_t::jfloat){
            f                   = is_neg_real ? -deci : deci;
        }
        else{
            u                   = is_neg_real ? u64(0) - real : real;
        }
        return json_string;
    }

    template<class item_t>
    struct core{
        using the_t = inc::cxx<item_t>;

        jsonx<item_t> parse_json(inc::ialloc<void> const & alloc){
            enum closure_t : uxx{
                in_object                   = uxx(json_type_t::jobject),
                in_array                    = uxx(json_type_t::jarray),
            };

            enum operation_t: uxx{
                fetch_key                   = in_object,
                fetch_value                 = in_array,
            };

            enum : uxx{
                mismatch,
                match,
            };

            using jap                       = json_arrayp<item_t>;
            using jop                       = json_objectp<item_t>;

            constexpr uxx json_depth        = 256;
            jap  stack[json_depth];
            char terminator[2];
            auto bytes                      = the.length() * sizeof(json_object<item_t>);
            auto buffer                     = alloc(bytes);
            auto buffer_end                 = u08p(buffer) + bytes;
            auto root                       = jap{};
            auto cur_lv                     = & stack[0];
            auto c                          = item_t('\0');
            auto except_next                = false;                // 遇到 ',' 逗号，表示还有下一个元素
            auto miss_terminator            = true;                 // 缺少终结符
            auto op                         = fetch_value;
            auto closure                    = in_array;
            auto type                       = json_type_t::unknwon;
            auto json_string                = static_cast<item_t const *>(the);
            auto buf_string                 = static_cast<item_t *>(buffer);
            auto buf_struct                 = static_cast<u08p /*字节数组*/>(buffer_end);

            // 子过程
            auto alloc_object               = [&](){
                buf_struct                 -= sizeof(json_object<item_t>);
                return xnew(buf_struct) json_object<item_t>();      // 初始化分配的内存
            };
            auto alloc_array                = [&](){
                buf_struct                 -= sizeof(json_array<item_t>);
                return xnew(buf_struct) json_array<item_t>();       // 初始化分配的内存
            };
            auto create_element             = [&](json_type_t type, voidp item, operation_t opr){
                cur_lv[0]->type(type);
                cur_lv[0]->value.u          = uxx(item);            // 赋值给联合体
                cur_lv[1]                   = jap(item);
                cur_lv                     += 1;
                json_string                += 1;
                closure                     = closure_t(type);      // closure_t 与 json_type_t 保持一致
                op                          = opr;
                except_next                 = false;                // 新创建子节点的首个元素忽略此状态
            };
            auto fetch_str                  = [&]() -> jsonx<item_t> {
                cur_lv[0]->type(json_type_t::jstring);
                cur_lv[0]->value.s          = buf_string;
                json_string                 = parse_string(& buf_string, json_string + 1/*skip '\"'*/);
                json_string                += 1;

                if (json_string[-1] == '\0'){
                    return { json_parse_result_t::unexpected_termination, json_string };
                }
                if (json_string[-1] != '\"'){ // 未知转义符
                    return { json_parse_result_t::unexpected_key_format, json_string };
                }
                else{
                    return { json_parse_result_t::success, json_string };
                }
            };
            auto fetch                      = [&](){
                if (json_string = parse_number(xref cur_lv[0]->value.u, xref type, json_string); 
                    type != json_type_t::unknwon){
                    cur_lv[0]->type(type);
                }
                else if (start_with(json_string, "true")){
                    json_string            += 4;
                    cur_lv[0]->type(json_type_t::jinteger);
                    cur_lv[0]->value.u      = 1;
                }
                else if (start_with(json_string, "false") or start_with(json_string, "null")){
                    json_string        += json_string[0] == 'f'/*is "false"*/ ? 5 : 4;
                    cur_lv[0]->type(json_type_t::jinteger);
                    cur_lv[0]->value.u  = 0;
                }
                else{
                    return mismatch;
                }
                return match;
            };

            terminator[in_object]           = '}';
            terminator[in_array]            = ']';
            cur_lv[0]                       = jap(& root);
            cur_lv[1]                       = jap(alloc_array()); // 与上文中的 closure 保持一致
            cur_lv[0]->type(json_type_t::jarray);
            cur_lv                         += 1;

            while(true){
                if (cur_lv == & stack[json_depth - 1]){
                    return { json_parse_result_t::depth_overflow, json_string };
                }
                if (json_string = skip_whitespace(json_string); json_string[0] == '\0'){
                    break;
                }
                if (op == fetch_value){
                    // 进入子节点
                    if (c = json_string[0]; c == '{'){
                        create_element(json_type_t::jobject, alloc_object(), fetch_key);
                        continue;
                    }
                    if (c == '['){
                        create_element(json_type_t::jarray, alloc_array(), fetch_value);
                        continue;
                    }

                    // 提取元素值
                    if (c == '\"'){
                        if (auto r = fetch_str(); r.parse_result() != json_parse_result_t::success){
                            return r;
                        }
                    }
                    else if (fetch() == mismatch and except_next){
                        return { json_parse_result_t::remainder_comma, json_string };
                    }

                    // 跳过空白字符
                    if (json_string = skip_whitespace(json_string); json_string[0] == '\0'){
                        return { json_parse_result_t::unexpected_termination, json_string };
                    }

                    // 退出子节点
                    for (miss_terminator = true;;){
                        if (c = json_string[0]; c != '}' and c != ']'){
                            break;
                        }

                        // 终结括号不匹配
                        if (c != terminator[closure]){
                            return { json_parse_result_t::terminator_mismatch, json_string };
                        }

                        // 栈底不能再退栈
                        if (cur_lv == & stack[0]){
                            return { json_parse_result_t::remainder_terminator, json_string };
                        }

                        cur_lv             -= 1;

                        // 当前节点的父节点存放着自己的类型
                        closure             = closure_t(cur_lv[-1]->type());
                        op                  = closure == in_object ? fetch_key : fetch_value;
                        json_string        += 1;
                        miss_terminator     = false; 

                        if (json_string = skip_whitespace(json_string); json_string[0] == '\0'){
                            break;
                        }
                    }

                    // 还存在元素，创建平级节点，并设置 except_next 期待下轮循环 fetch 到元素
                    if (c = json_string[0]; c == ','){ 
                        if (closure == in_object){
                            op              = fetch_key;
                            cur_lv[1]       = jap(alloc_object());
                        }
                        else{
                            cur_lv[1]       = jap(alloc_array());
                        }

                        // 旧的队列尾元素指向新元素
                        cur_lv[0]->next(cur_lv[1]);

                        // 新节点作为队列尾部
                        cur_lv[0]           = cur_lv[1];

                        json_string        += 1;
                        miss_terminator     = false;
                        except_next         = true;
                        continue;
                    }
                    else{
                        except_next         = false;
                    }

                    // 当前字符不是终结符('}', ']', ',')
                    if (miss_terminator){
                        return { json_parse_result_t::terminator_mismatch, json_string };
                    }
                    continue;
                }

                // 针对 json_type_t::jobject 的 fetch，先获取键，再在 fetch_value 中获取值
                if (op == fetch_key){
                    if (json_string[0] != '\"'){
                        return { json_parse_result_t::unexpected_key_format, json_string };
                    }

                    jop(cur_lv[0])->key     = static_cast<item_t *>(buf_string);

                    if (auto r = fetch_str(); r.parse_result() != json_parse_result_t::success){
                        return r;
                    }
                    if (json_string = skip_whitespace(json_string); json_string[0] != ':'){
                        return { json_parse_result_t::colon_mismatch, json_string };
                    }

                    json_string            += 1;
                    op                      = fetch_value;
                    continue;
                }
            }

            // 只有括号成对存在并满足正确的嵌套关系才能不会导致退栈失败
            if (cur_lv != & stack[1]){
                return { json_parse_result_t::terminator_missing, json_string };
            }
            return { cur_lv[0] };
        }
    };

    template<class final_t, class base, class item_t>
    struct meta : base{
        using base::base;
        using the_t = core<item_t>;

        jsonx<item_t> parse_json(inc::ialloc<void> const & alloc){
            return the.parse_json(alloc);
        }
    };
}

#endif

namespace mixc::lang_cxx_parse_json::xuser{
    using ::mixc::lang_cxx_parse_json::json_type_t;
    using ::mixc::lang_cxx_parse_json::json_parse_result_t;
    using ::mixc::lang_cxx_parse_json::json;
    using ::mixc::lang_cxx_parse_json::jsonx;

    template<class final_t, class item_t>
    using cxx = meta<final_t, xusing_lang_cxx::cxx<final_t, item_t>, item_t>;
}

#undef  xusing_lang_cxx
#define xusing_lang_cxx ::mixc::lang_cxx_parse_json::xuser
