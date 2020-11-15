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

using json_arrayp   = json_array *;
using json_objectp  = json_object *;

asciis skip_whitespace(asciis json_string){
    char whitespace[] = {
        char(0x09), char(0x0a), char(0x0b), char(0x0c), char(0x0d), char(0x20), char(0x85), char(0xa0),
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

    auto fetch_number = [&](uxx * ptr = nullptr){
        u64 value           = 0;
        uxx idx             = 0;

        if (ptr == nullptr){
            ptr             = & idx;
        }

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
        // error
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
        json_string        += 1;
        deci                = fetch_number(& i);
        deci                = expr10_unsafe(i) * deci + real;
        type[0]             = json_float_type;
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

void parse(u08p buffer, u08p buffer_end, asciis json_string){
    enum closure_t: uxx{
        in_object   = uxx(json_object_type),
        in_array    = uxx(json_array_type),
    };

    enum operation_t: uxx{
        fetch_key   = in_object,
        fetch_value = in_array,
    };

    using nodep = json_struct<void> *;

    nodep stack[256];
    char terminal[2];
    auto buf                        = buffer;
    auto cur_lv                     = & stack[0];
    auto c                          = '\0';
    auto op                         = fetch_value;
    auto closure                    = in_array;
    auto buf_string                 = buffer;
    auto buf_struct                 = buffer_end;
    auto type                       = json_unknwon_type;
    auto alloc_object               = [&](){
        buffer_end                 -= sizeof(json_object);
        return new(buffer_end) json_object();
    };
    auto alloc_array                = [&](){
        buffer_end                 -= sizeof(json_array);
        return new(buffer_end) json_array();
    };

    terminal[uxx(in_object)]        = '}';
    terminal[uxx(in_array)]         = ']';
    cur_lv[0]                       = alloc_array();

    while(json_string[0] != '\0'){
        if (json_string = skip_whitespace(json_string); json_string[0] == '\0'){
            break;
        }
        if (op == fetch_value){
            if (c = json_string[0]; c == '{'){
                cur_lv[0]->type(json_object_type);
                cur_lv[0]->value.o  = alloc_object();
                cur_lv[1]           = nodep(cur_lv[0]->value.o);
                cur_lv             += 1;
                closure             = in_object;
                op                  = fetch_key;
                continue;
            }
            if (c == '['){
                cur_lv[0]->type(json_array_type);
                cur_lv[0]->value.a  = alloc_array();
                cur_lv[1]           = nodep(cur_lv[0]->value.a);
                cur_lv             += 1;
                closure             = in_array;
                op                  = fetch_value;
                continue;
            }

            if (c == '\"'){
                cur_lv[0]->type(json_string_type);
                cur_lv[0]->value.s  = asciis(buf_string);
                json_string         = parse_string(& buf_string, json_string);

                if (json_string++; json_string[-1] != '\"'){
                    // error
                }
            }
            else if (json_string = parse_number(& cur_lv[0]->value.u, & type, json_string); type != json_unknwon_type){
                cur_lv[0]->type(type);
            }
            else if (start_with(json_string, "true")){
                cur_lv[0]->type(json_integer_type);
                cur_lv[0]->value.u  = 1;
            }
            else if (start_with(json_string, "false") or start_with(json_string, "null")){
                cur_lv[0]->type(json_integer_type);
                cur_lv[0]->value.u  = 0;
            }

            if (json_string = skip_whitespace(json_string); json_string[0] == '\0'){
                // error
            }

            if (c == '}' or c == ']'){
                if (c != terminal[closure]){ // 终结括号不匹配
                    // error
                }
                if (cur_lv == & stack[0]){ // 栈底不能再退栈
                    // error
                }

                cur_lv             -= 1;
                closure             = closure_t(cur_lv[0]->type());
                op                  = closure == in_object ? fetch_key : fetch_value;
                json_string        += 1;

                if (json_string = skip_whitespace(json_string); json_string[0] == '\0'){
                    break;
                }
            }

            if (c = json_string[0]; c == ','){ // 还存在元素
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
                continue;
            }
        }
        if (op == fetch_key){
            if (json_string[0] != '\"'){
                // error
            }
            if (json_string += 1; json_string[0] == '\0'){
                // error
            }

            json_objectp(cur_lv[0])->key = asciis(buf_string);
            json_string = parse_string(& buf_string, json_string); 

            if (json_string[0] != '\"'){
                // error
            }

            json_string            += 1; // skip '\"'
            json_string             = skip_whitespace(json_string);

            if (json_string[0] != ':'){
                // error
            }

            json_string            += 1;
            op                      = fetch_value;
            continue;
        }
    }
}
