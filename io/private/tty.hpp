#ifndef xpack_io_private_tty
#define xpack_io_private_tty
    #pragma push_macro("xuser")
        #undef  xuser
        #define xuser mixc::io_private_tty
        #include"define/base_type.hpp"
        #include"dumb/implicit.hpp"
        #include"io/private/tty_key_t.hpp"
        #include"macro/private/mix.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::io_private_tty{
        using print_t = int (*)(const char * fmt,...);
        extern print_t const  print_core;
        extern inc::tty_key_t read_key();
        extern void           print_begin();
        extern void           print_flush();

        struct ini{
            ini(){
                print_begin();
            }
        };

        inline ini __ini;

        template<class ... args>
        void print(args const & ... list){
            inc::implicit<inc::mix> items[] = { list... };
            for(auto item : items){
                print_core(item->fmt, item->v);
            }
        }
    }

#endif

namespace xuser::inc{
    using ::mixc::io_private_tty::print;
    using ::mixc::io_private_tty::print_flush;
    using ::mixc::io_private_tty::read_key;
}
