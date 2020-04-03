#ifndef xpack_macro_private_mix
#define xpack_macro_private_mix
    #pragma GCC diagnostic ignored "-Wempty-body"
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::macro_private_mix
        #include"define/base_type.hpp"
        #include"dumb/implicit.hpp"
        #include"meta/more_fit.hpp"
        #include<stdio.h>
    #pragma pop_macro("xuser")

    namespace mixc::macro_private_mix{
        struct mix{
            union{
                f64          f;
                u64          u;
                i64          i;
                char         c;
                const char * str;
                void *       v;
            };

            const char * fmt;

            template<class type>
            mix(type value){
                constexpr uxx index = inc::more_fit<
                    type, 
                    i08, 
                    i16, 
                    i32, 
                    i64, 
                    u08, 
                    u16, 
                    u32, 
                    u64, 
                    f32, 
                    f64, 
                    const char *,
                    void *,
                    char
                >;

                if constexpr (index < 4){
                    i   = value;
                    fmt = "%lld";
                }
                else if constexpr(index < 8){
                    u   = value;
                    fmt = "%llu";
                }
                else if constexpr(index < 10){
                    f   = value;
                    fmt = "%f";
                }
                else if constexpr(index < 11){
                    str = value;
                    fmt = "\"%s\"";
                }
                else if constexpr(index < 12){
                    v   = value;
                    fmt = "%p";
                }
                else if constexpr(index < 13){
                    c   = value;
                    fmt = "%c";
                }
            }

            template<class ... args>
            static void print(int no, const char * file, int line, const char * msg, args ... list){
                inc::implicit<mix> arg[] = { list... };
                printf("%5d | %s:%d\n      | ", no, file, line);

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
        };
    }
#endif
