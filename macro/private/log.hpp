#ifndef xpack_macro_private_log
#define xpack_macro_private_log
#pragma push_macro("xuser")
#undef  xuser
#define xuser mixc::macro_private_log::inc
#include"define/base_type.hpp"
#include"macro/private/mix.hpp"
#pragma pop_macro("xuser")

namespace mixc::macro_private_log::origin{
    enum log_type_t{
        for_debug,
        for_fail,
        for_test,
        for_hint,
    };

    enum message_type_t{
        success,
        normal,
        warning,
        fail,
    };

    extern void log(log_type_t log_type, asciis file, uxx line, asciis func_name, asciis message, message_type_t message_type);

    template<class a0_t, class ... args_t>
    inline void log(
        log_type_t          log_type, 
        asciis              file, 
        uxx                 line, 
        asciis              func_name, 
        asciis              message, 
        message_type_t      message_type,
        a0_t const &          first, 
        args_t const & ...    list){

        extern void log(
            log_type_t      log_type, 
            asciis          file, 
            uxx             line, 
            asciis          func_name, 
            asciis          message, 
            message_type_t  message_type,
            inc::mix *      items, 
            uxx             items_length
        );

        inc::mix items[] = { first, list... };
        log(log_type, file, line, func_name, message, message_type, items, 1 + sizeof...(args_t));
    }
}

#endif

xexport_space(mixc::macro_private_log::origin)