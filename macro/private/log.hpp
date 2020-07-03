#ifndef xpack_macro_private_log
#define xpack_macro_private_log
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_private_log
    #include"define/base_type.hpp"
    #include"macro/private/mix.hpp"
    #pragma pop_macro("xuser")

    namespace mixc::macro_private_log::origin{
        enum log_type_t{
            for_debug,
            for_fail,
            for_test,
        };

        extern void log(log_type_t id, asciis file, uxx line, asciis func_name, asciis message);

        template<class a0, class ... args>
        inline void log(log_type_t id, asciis file, uxx line, asciis func_name, asciis message, a0 const & first, args const & ... list){
            extern void log_core(
                log_type_t  type, 
                asciis      file, 
                uxx         line, 
                asciis      func_name, 
                asciis      message, 
                inc::mix *  items, 
                uxx         length
            );
            inc::mix arg[] = { first, list... };
            log_core(id, file, line, func_name, message, arg, 1 + sizeof...(args));
        }
    }

#endif

namespace xuser::inc{
    using namespace ::mixc::macro_private_log::origin;
}
