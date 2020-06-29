#ifndef xpack_macro_private_log_extern
#define xpack_macro_private_log_extern
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_private_log
    #include"configure.hpp"
    #include"define/base_type.hpp"
    #include"macro/private/log.hpp"
    #include"macro/private/mix.hpp"
    #include"macro/xdebug_fail.hpp"
    #include"io/tty.hpp"
    #include"lang/cxx/index_of_last.hpp"
    #include"lang/cxx.hpp"

    namespace xuser::origin{
        template<class ... args>
        void log_core(type_t id, asciis file, uxx line, asciis func_name, args const & ... contents){
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

            asciis type_list[2];
            type_list[for_debug] = "DBUG";
            type_list[for_fail ] = "FAIL";
            
            file += skip;
            tty.write(type_list[id], " | ", v{file, ':', line}.l(40), " | ", v{func_name}.l(20), " | ", contents...);
        }

        void log(type_t id, asciis file, uxx line, asciis func_name, asciis message){
            log_core(id, file, line, func_name, message, '\n');
        }

        void log_core(
            type_t      type, 
            asciis      file, 
            uxx         line, 
            asciis      func_name, 
            asciis      message, 
            inc::mix *  items, 
            uxx         length
        ){
            using namespace inc;
            log_core(type, file, line, func_name);

            for(uxx i = 0; i < length; i++, items++){
                bool is_origin_text = false;
                do {
                    if (message += 1; not is_origin_text){
                        is_origin_text = message[-1] == '\"';
                    }
                    tty.write(message[-1]);
                }while(message[0] != ',');

                if (message += 1; is_origin_text){
                    continue;
                }

                switch(items->fmt){
                case classify_type_t::is_char_t:        tty.write(':', items->c); break;
                // case classify_type_t::is_float_t:   tty.write(':', items->f); break;
                case classify_type_t::is_ptr_t:         tty.write(':', items->v); break;
                case classify_type_t::is_signed_t:      tty.write(':', items->i); break;
                case classify_type_t::is_str_t:         tty.write(':', items->s); break;
                case classify_type_t::is_unsigned_t:    tty.write(':', items->u); break;
                default: 
                    xdebug_fail("classify_type_t miss match");
                    break;
                }
            }
            tty.write_line();
        }
    }
    #pragma pop_macro("xuser")
#endif
