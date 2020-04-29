#ifndef xpack_macro_private_log
#define xpack_macro_private_log
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_private_log
        #include"define/base_type.hpp"
        #include"dumb/implicit.hpp"
        #include"macro/private/mix.hpp"
        #include<stdio.h>
        #include<string.h>
    #pragma pop_macro("xuser")

    namespace mixc::macro_private_log{
        template<class ... args>
        inline void log(int no, const char * file, int line, const char * msg, args ... list){
            #ifdef xdebug_short_path
                char prefix[] = "Mix-C";
                while(true) {
                    auto name = strstr(file, prefix);
                    if (name != nullptr) {
                        file = name + sizeof(prefix);
                    }
                    else {
                        break;
                    }
                }
            #endif

            inc::implicit<inc::mix> arg[] = { list... };
            char buf[128] = {0};
            sprintf(buf, "%5d | %s:%d ", no, file, line); // may over range =============================

            for(int i = strlen(buf); i < 40; i++){
                buf[i] = ' ';
            }
            
            printf("%s | ", buf);

            for (auto item : arg){
                do {
                    msg += 1;
                    putchar(msg[-1]);
                }while(msg[0] != ',');

                putchar(':');
                printf(item->fmt, item->v);
            }
            putchar('\n');
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::macro_private_log::log;
}
