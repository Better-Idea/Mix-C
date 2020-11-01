#ifndef xpack_macro_private_log_extern
#define xpack_macro_private_log_extern
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_private_log_extern::inc
#include"configure.hpp"
#include"define/base_type.hpp"
#include"macro/private/log.hpp"
#include"macro/private/mix.hpp"
#include"macro/xexport.hpp"
#include"macro/xdebug_fail.hpp"
#include"macro/xdefer.hpp"
#include"io/tty.hpp"
#include"lang/cxx/index_of_last.hpp"
#include"lang/cxx/ph.hpp"
#include"lang/cxx.hpp"
#include"lock/mutex.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_private_log::inc{
    using namespace ::mixc::macro_private_log_extern::inc;
}

namespace mixc::macro_private_log::origin{
    template<class ... args>
    void log_core(log_type_t id, asciis file, uxx line, asciis func_name, args const & ... contents){
        using namespace inc;
        using namespace inc::ph;
        static uxx skip = 0;

        #if xuse_xdebug_short_path
            if (skip == 0){
                if (auto i = c08{file}.index_of_last(xmixc); i != not_exist){
                    skip  = i + sizeof(xmixc);
                    skip += file[skip] == '/' or file[skip] == '\\';
                }
            }
        #endif

        asciis type_list[4];
        type_list[for_debug] = "DBUG";
        type_list[for_fail ] = "FAIL";
        type_list[for_test ] = "TEST";
        type_list[for_hint ] = "HINT";
        
        file += skip;
        tty.write(type_list[id], " | ", v{file, ':', line}.l(40), " | ", v{func_name}.l(20), " | ", contents...);
    }

    void log(log_type_t id, asciis file, uxx line, asciis func_name, asciis message){
        log_core(id, file, line, func_name, message, '\n');
    }

    inc::mutex print_mutex;

    void log_core(
        log_type_t  type, 
        asciis      file, 
        uxx         line, 
        asciis      func_name, 
        asciis      message, 
        inc::mix *  items, 
        uxx         length
    ){
        using namespace inc;

        xdefer{
            print_mutex.unlock();
        };
        print_mutex.lock();

        log_core(type, file, line, func_name);

        for(uxx i = 0; i < length; i++, items++){
            bool is_origin_text = false;
            uxx  brackets       = 0;

            do {
                if (message += 1; not is_origin_text){
                    switch(message[-1]){
                    case '(':  brackets++;              break;
                    case ')':  brackets--;              break;
                    case '\"': is_origin_text = true;   break;
                    }

                    tty.write(message[-1]);
                }
                else switch(message[-1]){
                case '\\':
                    tty.write("\\", message[0]);
                    message += 1;
                    break;
                case '\"':
                    tty.write("\"");

                    if (brackets != 0){
                        is_origin_text = false;
                    }
                    break;
                default:
                    tty.write(message[-1]);
                    break;
                }

                // message 表达式可能包含函数调用，小括号间包含个 ',' 分隔符
                // 例如：message = "a, b, c.call(a, \"hello\", c),"
                // 但需要下列表达式同时满足时才表示结束语义
            }while(not (message[0] == ',' and brackets == 0));

            if (message += 1; is_origin_text){
                continue;
            }

            switch(items->fmt){
            case classify_type_t::is_char_t:        tty.write(':', items->c); break;
            case classify_type_t::is_float_t:       tty.write(':', items->f); break;
            case classify_type_t::is_ptr_t:         tty.write(':', items->v); break;
            case classify_type_t::is_signed_t:      tty.write(':', items->i); break;
            case classify_type_t::is_unsigned_t:    tty.write(':', items->u); break;
            case classify_type_t::is_str_t:
                tty.write(':', "\"", items->slen == not_exist ? 
                    inc::c08{items->s} : inc::c08{items->s, items->slen}, 
                    "\""
                );
                break;
            default: 
                xdebug_fail("classify_type_t miss match");
                break;
            }
        }
        tty.write_line();
    }
}

#endif
