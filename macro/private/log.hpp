#ifndef xpack_macro_private_log
#define xpack_macro_private_log
    #pragma push_macro("xuser")
    #undef  xuser
    #define xuser mixc::macro_private_log
    #include"configure.hpp"
    #include"define/base_type.hpp"
    #include"dumb/implicit.hpp"
    #include"macro/private/mix.hpp"
    
    namespace xuser::origin{
        enum type_t{
            for_debug,
            for_fail,
        };
        extern void log_core(
            type_t      type, 
            asciis      file, 
            uxx         line, 
            asciis      func_name, 
            asciis      message, 
            inc::mix *  items, 
            uxx         length
        );

        extern void log(type_t id, asciis file, uxx line, asciis func_name, asciis message);

        template<class a0, class ... args>
        inline void log(type_t id, asciis file, uxx line, asciis func_name, asciis message, a0 const & first, args const & ... list){
            inc::mix arg[] = { first, list... };
            log_core(id, file, line, func_name, message, arg, 1 + sizeof...(args));
        }
    }

    #pragma pop_macro("xuser")
#endif

namespace xuser::inc{
    using ::mixc::macro_private_log::origin::log;
}
