#ifndef xpack_io_privaye_printf
#define xpack_io_privaye_printf
    namespace mixc::io_private_printf{
        using print_t = int (*)(const char * fmt,...);
        extern print_t const print_core;

        template<class ... args>
        void print(const char * fmt, args const & ... list){
            print_core(fmt, args(list)...);
        }

        void print_flush();
    }
#endif

namespace xuser::inc{
    using mixc::io_private_printf::print;
    using mixc::io_private_printf::print_flush;
}
