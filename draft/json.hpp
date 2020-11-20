#define xuser mixc::leon::inc
#include"math/expr10.hpp"
#include"math/exp10.hpp"
#include"macro/xnew.hpp"
using namespace xuser;

struct json_object;
struct json_array;

enum json_type_t : uxx{
    json_object_type,
    json_array_type,
    json_float_type,
    json_integer_type,
    json_string_type,
    json_unknwon_type,
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

union json_value{
    asciis              s;
    f64                 f;
    u64                 u;
    i64                 i;
    json_array  *       a;
    json_object *       o = nullptr;
};

template<class next_t>
struct json_struct{
    json_value          value;

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

struct json_array : json_struct<json_array>{};
struct json_object : json_struct<json_object>{
    asciis              key = "";
};

struct json{
    json(json_array * ptr) : ptr(ptr){}
    json(json_parse_result_t state, asciis location_of_error) : 
        state(state), location_of_error(location_of_error){
    }

    json_parse_result_t state   = json_parse_result_t::success;

    union{
        asciis          location_of_error;
        json_array *    ptr;
    };

    auto operator->(){
        return ptr;
    }
};

using json_arrayp   = json_array *;
using json_objectp  = json_object *;

asciis skip_whitespace(asciis json_string){
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

bool start_with(asciis json_string, asciis value){
    while(json_string[0] == value[0]){
        json_string    += 1;
        value          += 1;
    }
    return value[0] == '\0';
}

asciis parse_string(u08p * buffer, asciis json_string){
    char * buf = (char *)*buffer;

    for(; json_string[0] != '\0' and json_string[0] != '\"'; json_string++){
        if (json_string[0] != '\\'){
            buf[0] = json_string[0];
            buf   += 1;
            continue;
        }

        if (json_string += 1; json_string[0] == '\0'){
            // error
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
            // to convert
            break;
        default:
            // error
            break;
        }
    }
    buf[0]      = '\0';
    buf        += 1;
    buffer[0]   = u08p(buf);
    return json_string;
}

asciis parse_number(void * buffer, json_type_t * type, asciis json_string){
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

        type[0]             = json_unknwon_type;
        return value;
    };

    type[0]                 = json_integer_type;
    auto is_neg_real        = is_neg();
    auto is_neg_exp         = false;
    auto i                  = uxx(0);
    auto exp                = uxx(0);
    auto real               = fetch_number();
    auto deci               = 0.0;
    auto & u                = u64p(buffer)[0];
    auto & f                = f64p(buffer)[0];

    if (type[0] == json_unknwon_type){
        return json_string;
    }
    if (json_string[0] == '.'){
        type[0]             = json_float_type;
        json_string        += 1;
        deci                = fetch_number(& i);
        deci                = expr10_unsafe(i) * deci + real;
    }
    if (json_string[0] == 'e' or json_string[0] == 'E'){
        json_string        += 1;
        is_neg_exp          = is_neg();
        exp                 = fetch_number(& i);

        if (exp > 307){ // max exp
            type[0]         = json_unknwon_type;
            return json_string;
        }
        deci               *= is_neg_exp ? expr10(exp) : exp10(exp);
    }

    if (type[0] == json_float_type){
        f                   = is_neg_real ? -deci : deci;
    }
    else{
        u                   = is_neg_real ? u64(0) - real : real;
    }
    return json_string;
}

json parse_json(voidp buffer, voidp buffer_end, asciis json_string){
    enum closure_t: uxx{
        in_object   = uxx(json_object_type),
        in_array    = uxx(json_array_type),
    };

    enum operation_t: uxx{
        fetch_key   = in_object,
        fetch_value = in_array,
    };

    enum : uxx{
        mismatch,
        match,
    };

    using nodep = json_struct<void> *;

    constexpr uxx json_depth        = 256;
    nodep stack[json_depth];
    char terminator[2];
    auto json_string_begin          = json_string;
    auto root                       = json_array{};
    auto buf                        = buffer;
    auto cur_lv                     = & stack[0];
    auto c                          = '\0';
    auto op                         = fetch_value;
    auto except_next                = false;                // 遇到 ',' 逗号，表示还有下一个元素
    auto miss_terminator            = true;                 // 缺少终结符
    auto closure                    = in_array;
    auto buf_string                 = u08p(buffer);
    auto buf_struct                 = u08p(buffer_end);
    auto type                       = json_unknwon_type;

    // 子过程
    auto alloc_object               = [&](){
        buf_struct                 -= sizeof(json_object);
        return xnew(buf_struct) json_object();              // 初始化分配的内存
    };
    auto alloc_array                = [&](){
        buf_struct                -= sizeof(json_array);
        return xnew(buf_struct) json_array();               // 初始化分配的内存
    };
    auto create_element             = [&](json_type_t type, voidp item, operation_t opr){
        cur_lv[0]->type(type);
        cur_lv[0]->value.u          = uxx(item);            // 赋值给联合体
        cur_lv[1]                   = nodep(item);
        cur_lv                     += 1;
        json_string                += 1;
        closure                     = closure_t(type);      // closure_t 与 json_type_t 保持一致
        op                          = opr;
        except_next                 = false;                // 新创建子节点的首个元素忽略此状态
    };
    auto fetch                      = [&](){
        if (c == '\"'){
            cur_lv[0]->type(json_string_type);
            cur_lv[0]->value.s  = asciis(buf_string);
            json_string         = parse_string(& buf_string, json_string + 1/*skip '\"'*/);

            if (json_string++; json_string[-1] != '\"'){
                // error
            }
        }
        else if (json_string = parse_number(& cur_lv[0]->value.u, & type, json_string); type != json_unknwon_type){
            cur_lv[0]->type(type);
        }
        else if (start_with(json_string, "true")){
            json_string        += 4;
            cur_lv[0]->type(json_integer_type);
            cur_lv[0]->value.u  = 1;
        }
        else if (start_with(json_string, "false") or start_with(json_string, "null")){
            json_string        += json_string[0] == 'f'/*is "false"*/ ? 5 : 4;
            cur_lv[0]->type(json_integer_type);
            cur_lv[0]->value.u  = 0;
        }
        else{
            return mismatch;
        }
        return match;
    };

    terminator[in_object]           = '}';
    terminator[in_array]            = ']';
    cur_lv[0]                       = nodep(& root);
    cur_lv[1]                       = nodep(alloc_array()); // 与上文中的 closure 保持一致
    cur_lv[0]->type(json_array_type);
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
                create_element(json_object_type, alloc_object(), fetch_key);
                continue;
            }
            if (c == '['){
                create_element(json_array_type, alloc_array(), fetch_value);
                continue;
            }

            // 提取元素值
            if (fetch() == mismatch and except_next){
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
                    cur_lv[1]       = nodep(alloc_object());
                }
                else{
                    cur_lv[1]       = nodep(alloc_array());
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

        // 针对 json_object_type 的 fetch，先获取键，再在 fetch_value 中获取值
        if (op == fetch_key){
            if (json_string[0] != '\"'){
                return { json_parse_result_t::unexpected_key_format, json_string };
            }
            if (json_string += 1; json_string[0] == '\0'){
                return { json_parse_result_t::unexpected_termination, json_string };
            }

            json_objectp(cur_lv[0])->key 
                                    = asciis(buf_string);
            json_string             = parse_string(& buf_string, json_string); 

            if (json_string[0] != '\"'){
                return { json_parse_result_t::unexpected_key_format, json_string };
            }

            json_string            += 1; // skip '\"'
            json_string             = skip_whitespace(json_string);

            if (json_string[0] != ':'){
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
    return { json_arrayp(cur_lv[0]) };
}
